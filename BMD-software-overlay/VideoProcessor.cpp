#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>
#include <thread>         // std::thread
#include <intrin.h>

volatile uint8_t * currentFrame;

// Source: http://www.alfredklomp.com/programming/sse-intrinsics/
static inline __m512i
_mm512_div255_epu16(__m512i x)
{
	// Divide 8 16-bit uints by 255:
	// x := ((x + 1) + (x >> 8)) >> 8:
	return _mm512_srli_epi16(_mm512_adds_epu16(
		_mm512_adds_epu16(x, _mm512_set1_epi16(1)),
		_mm512_srli_epi16(x, 8)), 8);
}

// Probably too big to want to inline:
static __m512i
_mm512_scale_epu8(__m512i x, __m512i y)
{
	// Returns an "alpha blend" of x scaled by y/255;
	//   x := x * (y / 255)
	// Reorder: x := (x * y) / 255

	// Unpack x and y into 16-bit uints:
	__m512i xlo = _mm512_unpacklo_epi8(x, _mm512_setzero_si512());
	__m512i ylo = _mm512_unpacklo_epi8(y, _mm512_setzero_si512());
	__m512i xhi = _mm512_unpackhi_epi8(x, _mm512_setzero_si512());
	__m512i yhi = _mm512_unpackhi_epi8(y, _mm512_setzero_si512());

	// Multiply x with y, keeping the low 16 bits:
	xlo = _mm512_mullo_epi16(xlo, ylo);
	xhi = _mm512_mullo_epi16(xhi, yhi);

	// Divide by 255: (custom functie, zie boven)
	xlo = _mm512_div255_epu16(xlo);
	xhi = _mm512_div255_epu16(xhi);

	// Repack the 16-bit uints to clamped 8-bit values:
	return _mm512_packus_epi16(xlo, xhi);
}

// load 4 alpha values into 512 bit register
 inline __m512i createAlphaVector(uint8_t* overlay) {
	 uint8_t* scalars = (uint8_t*) malloc(64); //64 bytes = 512 bits

	 for (int i = 0; i < 16; ++i) {
		 uint8_t pix = 255 - *(overlay + 4*i + 3);
		 for (int j = 0; j < 4; ++j) {
			 *(scalars + j +  4*i) = pix;
		 }
	 }

	 __m512i result = _mm512_load_si512(scalars);
	free(scalars); // release temporary buffer
	return result;
}


void processBlock(uint8_t* overlayPtr, long startOffset, int numBytes) {

		uint8_t* frameDataPointer = const_cast<uint8_t*>(currentFrame); // save volatile var locally
		frameDataPointer = frameDataPointer + startOffset;
		overlayPtr = overlayPtr + startOffset;

		for (int j = 0; j < numBytes; j = j + 64) {
			uint8_t* pixelPtr = frameDataPointer + j ;
			uint8_t* overlayPixelPtr = overlayPtr + j;
			
			// fil vector with alpha values to make one-by-one multiplication possible
			__m512i alpha = createAlphaVector(overlayPixelPtr);

			//scale incoming image with alpha value
			__m512i multiplied = _mm512_scale_epu8(*(__m512i*) pixelPtr, alpha);

			//add overlay and image
			_mm512_store_si512((__m512i*)pixelPtr, _mm512_add_epi8(multiplied, *(__m512i*)overlayPixelPtr));
		}
}

VideoProcessor::VideoProcessor() {
	assert(PIXEL_MODE == BMDPixelFormat::bmdFormat8BitBGRA); // only this supported for now
	imageSource = new ImageSource();
	overlayPtr = imageSource->getImage();

}

void VideoProcessor::processFrame(IDeckLinkVideoFrame * frame) {
	long height = frame->GetHeight();
	long width = frame->GetWidth();
	long rowWords = frame->GetRowBytes();

	frame->GetBytes((void**)&currentFrame);
	int numBytes = (rowWords * height) / NUM_THREADS;
	
	std::thread* myThreads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; ++i) {
		long startOffset = numBytes * i;
		myThreads[i] = new std::thread(processBlock, overlayPtr, startOffset, numBytes);
	}

	for (int i = 0; i < NUM_THREADS; ++i) {
		myThreads[i]->join();
		delete myThreads[i];
	}
}


void VideoProcessor::publishFrame(IDeckLinkVideoFrame * frame)
{
	processFrame(frame);

	int fail = output->showFrame(frame);
	if (fail) printf("Error outputting frame\n");
}

void VideoProcessor::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}