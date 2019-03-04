#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	assert(PIXEL_MODE == BMDPixelFormat::bmdFormat8BitARGB); // only this supported for now
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
		uint8_t* overlayRowPtr = overlayPtr + i * 4 * width;

		for (int j = 0; j < width; j++) {
			uint32_t* pixelPtr = rowPtr + j;
			uint8_t* overlayPixelPtr = overlayRowPtr + j * 4;
			//*pixelPtr = *pixelPtr & 0xFFFFFFFF;
			*pixelPtr = (*(overlayPixelPtr) << 24)
				| (*(overlayPixelPtr + 1) << 16)
				| (*(overlayPixelPtr + 2) << 8)
				| *(overlayPixelPtr + 3);

			// Twee karakters per component BGRA
			//*pixelPtr = 0x0000FFFF; // dit geeft rood
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
