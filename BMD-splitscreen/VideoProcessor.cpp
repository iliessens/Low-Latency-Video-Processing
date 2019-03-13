#include "VideoProcessor.h"
#include <assert.h>
#include <future>
#include <intrin.h>

VideoProcessor::VideoProcessor() {

}

VideoProcessor::~VideoProcessor()
{

}

bool VideoProcessor::processFrame(IDeckLinkVideoFrame * frame) {
	return true;
}


void VideoProcessor::publishFrame(IDeckLinkVideoFrame * frame, char stream)
{
	if (stream == 1) {
		output->showFrame(frame);
	}
	else {
		frame->Release();
	}
}

void VideoProcessor::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}