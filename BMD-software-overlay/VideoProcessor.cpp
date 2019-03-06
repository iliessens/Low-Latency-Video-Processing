#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>
#include <thread>         // std::thread

volatile uint8_t * currentFrame;

void processBlock(uint8_t* overlayPtr, long startOffset, int numBytes) {

		uint8_t* frameDataPointer = const_cast<uint8_t*>(currentFrame); // save volatile var locally
		frameDataPointer = frameDataPointer + startOffset;
		overlayPtr = overlayPtr + startOffset;

		for (int j = 0; j < numBytes; j = j + 4) {
			uint8_t* pixelPtr = frameDataPointer + j ;
			uint8_t* overlayPixelPtr = overlayPtr + j;

			uint8_t w = 255 - *(overlayPixelPtr + 3); // 1 - alpha

			for (int k = 0; k < IMAGE_CHANNELS - 1; ++k) {
				uint16_t temp = *(pixelPtr + k) * w;
				*(pixelPtr + k) = (temp >> 8) + *(overlayPixelPtr + k); // divide by 255 and add
			}
			//*(pixelPtr) = 0xFF;
			//*(pixelPtr + 1) = 0;
			//*(pixelPtr + 2) = 0;
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
