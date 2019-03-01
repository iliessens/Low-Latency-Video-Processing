#include "VideoProcessor.h"
#include "settings.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	assert(PIXEL_MODE == BMDPixelFormat::bmdFormat8BitBGRA); // only this supported for now
}

void VideoProcessor::publishFrame(IDeckLinkVideoFrame * frame)
{
	long height = frame->GetHeight();
	long width = frame->GetWidth();
	long dataLength = width * height;

	int* frameDataPointer;
	frame->GetBytes((void**) &frameDataPointer);

	for (int i = 0; i < dataLength; i = i + 2) {
		*(frameDataPointer + i) = 0; // set every second pixel black
	}

	int fail = output->showFrame(frame);
	if (fail) printf("Error outputting frame\n");
}

void VideoProcessor::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}
