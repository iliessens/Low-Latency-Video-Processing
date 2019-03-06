#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	assert(PIXEL_MODE == BMDPixelFormat::bmdFormat8BitBGRA); // only this supported for now
	imageSource = new ImageSource();
	overlayPtr = imageSource->getImage();
}

void VideoProcessor::processFrame(IDeckLinkVideoFrame * frame) {
	long height = frame->GetHeight();
	long width = frame->GetWidth();
	long rowWords = frame->GetRowBytes();

	uint8_t* frameDataPointer;
	frame->GetBytes((void**)&frameDataPointer);

	for (int i = 0; i < height; ++i) {
		uint8_t* rowPtr = frameDataPointer + i * rowWords;
		uint8_t* overlayRowPtr = overlayPtr + i * 4 * width;

		for (int j = 0; j < rowWords; j = j + 4) {
			uint8_t* pixelPtr = rowPtr + j;
			uint8_t* overlayPixelPtr = overlayRowPtr + j;

			uint8_t w = 255 - *(overlayPixelPtr + 3); // 1 - alpha

			for (int k = 0; k < IMAGE_CHANNELS - 1; ++k) {
				uint16_t temp = *(pixelPtr + k) * w;
				*(pixelPtr + k) = (temp >> 8) + *(overlayPixelPtr + k); // divide by 255 and add
			}

			//*pixelPtr = *overlayPixelPtr; // B
			//*(pixelPtr + 1) = *(overlayPixelPtr + 1); // G
			//*(pixelPtr + 2) = *(overlayPixelPtr + 2); // R
			//*(pixelPtr + 3) = *(overlayPixelPtr + 3); // A

			//uint8_t alpha = ((*overlayPixelPtr) & 0xFF000000) >> 24;
			//if (alpha) *pixelPtr = *overlayPixelPtr;
		}
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
