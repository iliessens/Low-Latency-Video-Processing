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

void processBlock(uint8_t* overlayPtr, long startOffset, int numBytes) {

		uint8_t* frameDataPointer = const_cast<uint8_t*>(currentFrame); // save volatile var locally
		frameDataPointer = frameDataPointer + startOffset;
		overlayPtr = overlayPtr + startOffset;

		for (int j = 0; j < numBytes; j = j + 64) {
			uint8_t* pixelPtr = frameDataPointer + j ;
			uint8_t* overlayPixelPtr = overlayPtr + j;

			for (int i = 0; i <= 16; ++i) { // preprocess 16 pixels

				uint8_t w = 255 - *(overlayPixelPtr + 3 + i * 4); // 1 - alpha

				for (int k = 0; k < IMAGE_CHANNELS - 1; ++k) {
					uint16_t temp = *(pixelPtr + k + i*4) * w;
					*(pixelPtr + k + i*4) = (temp >> 8); // divide by 255 and add
				}
			}

			_mm512_store_si512((__m512i*) pixelPtr, _mm512_add_epi8(*(__m512i*)pixelPtr, *(__m512i*)  overlayPixelPtr));
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