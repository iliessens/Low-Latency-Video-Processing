#include "VideoProcessor.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&converter);
	converter->AddRef();
}

VideoProcessor::~VideoProcessor()
{
	converter->Release();
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
		IDeckLinkMutableVideoFrame* rgbframe;
		output->getEmptyFrame(&rgbframe);
		
		converter->ConvertFrame(frame, rgbframe);
		frame->Release();

		output->showFrame(rgbframe);
	}
}
