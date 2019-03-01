#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	assert(PIXEL_MODE == BMDPixelFormat::bmdFormat8BitARGB); // only this supported for now
}

void VideoProcessor::processRow(uint32_t* rowData, int width) {

	for (int i = 0; i < width; i++) {
		uint32_t* pixelPtr = rowData + i;
		*pixelPtr = *pixelPtr & 0xFFFFFFFF;
	}
}

void VideoProcessor::publishFrame(IDeckLinkVideoFrame * frame)
{
	long height = frame->GetHeight();
	long width = frame->GetWidth();
	long rowWords = frame->GetRowBytes() / 4; // 4 bytes in int

	uint32_t* frameDataPointer;
	frame->GetBytes((void**) &frameDataPointer);

	for (int i = 0; i < height; i++) {
		uint32_t* rowPtr = frameDataPointer + i * rowWords;
		this->processRow(rowPtr, width);
	}

	int fail = output->showFrame(frame);
	if (fail) printf("Error outputting frame\n");
}

void VideoProcessor::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}
