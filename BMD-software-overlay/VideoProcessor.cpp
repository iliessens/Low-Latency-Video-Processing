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
	long rowWords = frame->GetRowBytes() / 4; // 4 bytes in int32

	uint32_t* frameDataPointer;
	frame->GetBytes((void**)&frameDataPointer);

	for (int i = 0; i < height; i++) { //
		uint32_t* rowPtr = frameDataPointer + i * rowWords;
		uint32_t* overlayRowPtr = overlayPtr + i * width;

		for (int j = 0; j < width; j++) {
			uint32_t* pixelPtr = rowPtr + j;
			uint32_t* overlayPixelPtr = overlayRowPtr + j;

			uint8_t alpha = ((*overlayPixelPtr) & 0xFF000000) >> 24;
			if (alpha) *pixelPtr = *overlayPixelPtr;

			// Twee karakters per component ARGB
			//*pixelPtr = 0xFFFF0000; // dit geeft rood
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
