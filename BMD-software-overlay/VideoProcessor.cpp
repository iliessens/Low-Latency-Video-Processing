#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>
#include <thread>         // std::thread
#include <intrin.h>

volatile uint8_t * currentFrame;

// Source: http://www.alfredklomp.com/programming/sse-intrinsics/
static inline __m128i
_mm_div255_epu16(__m128i x)
{
	// Divide 8 16-bit uints by 255:
	// x := ((x + 1) + (x >> 8)) >> 8:
	return _mm_srli_epi16(_mm_adds_epu16(
		_mm_adds_epu16(x, _mm_set1_epi16(1)),
		_mm_srli_epi16(x, 8)), 8);
}

// Probably too big to want to inline:
static __m128i
_mm_scale_epu8(__m128i x, __m128i y)
{
	// Returns an "alpha blend" of x scaled by y/255;
	//   x := x * (y / 255)
	// Reorder: x := (x * y) / 255

	// Unpack x and y into 16-bit uints:
	__m128i xlo = _mm_unpacklo_epi8(x, _mm_setzero_si128());
	__m128i ylo = _mm_unpacklo_epi8(y, _mm_setzero_si128());
	__m128i xhi = _mm_unpackhi_epi8(x, _mm_setzero_si128());
	__m128i yhi = _mm_unpackhi_epi8(y, _mm_setzero_si128());

	// Multiply x with y, keeping the low 16 bits:
	xlo = _mm_mullo_epi16(xlo, ylo);
	xhi = _mm_mullo_epi16(xhi, yhi);

	// Divide by 255:
	xlo = _mm_div255_epu16(xlo);
	xhi = _mm_div255_epu16(xhi);

	// Repack the 16-bit uints to clamped 8-bit values:
	return _mm_packus_epi16(xlo, xhi);
}

// load 4 alpha values into 128 bit register
 inline __m128i createAlphaVector(uint8_t* overlay) {
	const uint8_t pix1 = 255 - *(overlay + 3);
	const uint8_t pix2 = 255 - *(overlay + 7);
	const uint8_t pix3 = 255 - *(overlay + 11);
	const uint8_t pix4 = 255 - *(overlay + 15);
	const __m128i scalar = _mm_setr_epi8(pix1, pix1, pix1, pix1,
		pix2, pix2, pix2, pix2,
		pix3, pix3, pix3, pix3,
		pix4, pix4, pix4, pix4);

	return scalar;
}


void processBlock(uint8_t* overlayPtr, long startOffset, int numBytes) {

		uint8_t* frameDataPointer = const_cast<uint8_t*>(currentFrame); // save volatile var locally
		frameDataPointer = frameDataPointer + startOffset;
		overlayPtr = overlayPtr + startOffset;

		for (int j = 0; j < numBytes; j = j + 16) {
			uint8_t* pixelPtr = frameDataPointer + j ;
			uint8_t* overlayPixelPtr = overlayPtr + j;
			
			// fil vector with alpha values to make one-by-one multiplication possible
			__m128i alpha = createAlphaVector(overlayPixelPtr);

			//scale incoming image with alpha value
			__m128i multiplied = _mm_scale_epu8(*(__m128i*) pixelPtr, alpha);

			// write result to image memory
			_mm_store_si128((__m128i*) pixelPtr, multiplied);

		}

		//Second loop for instructions that process multiple pixels
		for (int j = 0; j < numBytes; j = j + 64) {
			uint8_t* pixelPtr = frameDataPointer + j;
			uint8_t* overlayPixelPtr = overlayPtr + j;
			_mm512_store_si512((__m512i*)pixelPtr, _mm512_add_epi8(*(__m512i*)pixelPtr, *(__m512i*)overlayPixelPtr));
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