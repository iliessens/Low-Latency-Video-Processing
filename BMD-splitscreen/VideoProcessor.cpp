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
	IDeckLinkMutableVideoFrame* composite;
	output->getEmptyFrame(&composite);
	if (!composite) return;
	//Prevent working with invalid frames

	uint16_t* outbytes;
	uint16_t* inLeft;
	uint16_t* inRight;

	frame1->GetBytes((void**) &inLeft);
	frame2->GetBytes((void**) &inRight);
	composite->GetBytes((void**)&outbytes);

	for (int i = 0; i < composite->GetHeight(); ++i) {
		uint16_t* lRowPtr = inLeft + WIDTH * i;
		uint16_t* dstRowPtr = outbytes + WIDTH * i;

		// do left part
		uint16_t* lstartPtr = lRowPtr + (WIDTH / 4);
		uint16_t* dstPtr = dstRowPtr;

		memcpy(dstPtr, lstartPtr, WIDTH);

		// do right part
		uint16_t* rstartPtr = (inRight + WIDTH * i) + WIDTH / 4;
		dstPtr = dstRowPtr + WIDTH / 2;
		memcpy(dstPtr, rstartPtr, WIDTH);
		
	}

	output->showFrame(composite);
	frame1->Release();
	frame2->Release();
}
