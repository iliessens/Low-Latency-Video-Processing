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
		//output->showFrame(frame);
		frame1 = frame;
	}
	else {
		/*IDeckLinkMutableVideoFrame* rgbframe;
		output->getEmptyFrame(&rgbframe);
		
		converter->ConvertFrame(frame, rgbframe);*/

		frame2 = frame;
	}
}

void VideoProcessor::trigger()
{
	if ((!frame1) || (!frame2)) return;
	IDeckLinkMutableVideoFrame* composite;
	output->getEmptyFrame(&composite);

	uint32_t* outbytes;
	uint32_t* inLeft;
	uint32_t* inRight;

	frame1->GetBytes((void**) &inLeft);
	frame2->GetBytes((void**) &inRight);
	composite->GetBytes((void**)&outbytes);

	for (int i = 0; i < composite->GetHeight(); ++i) {
		uint32_t* lRowPtr = inLeft + WIDTH * (i / 2);
		uint32_t* dstRowPtr = (outbytes + WIDTH * (i/2));

		// do left part
		uint32_t* lstartPtr = lRowPtr + (WIDTH / 8);
		uint32_t* dstPtr = dstRowPtr;

		memcpy(dstPtr, lstartPtr, WIDTH / 4);

		// do right part
		uint32_t* rstartPtr = (inRight + WIDTH * i/2) + WIDTH / 8;
		dstPtr = (outbytes + WIDTH * i/2 + WIDTH / 4);
		memcpy(dstPtr, rstartPtr, WIDTH / 2);
		
	}

	output->showFrame(composite);
	frame1->Release();
	frame2->Release();
}
