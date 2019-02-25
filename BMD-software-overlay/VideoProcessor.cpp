#include "VideoProcessor.h"

VideoProcessor::VideoProcessor() {

}

void VideoProcessor::publishFrame(IDeckLinkVideoFrame * frame)
{
	// passthrough
	int fail = output->showFrame(frame);
	if (fail) printf("Error outputting frame\n");
}

void VideoProcessor::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}
