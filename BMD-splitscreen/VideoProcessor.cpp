#include "VideoProcessor.h"
#include <assert.h>

VideoProcessor::VideoProcessor() {
	CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&converter);
	converter->AddRef();
}

void VideoProcessor::setOutput(DeckLinkOutputDevice* output) {
	AbstractVideoProcessor::setOutput(output);
	output->getEmptyFrame(&composite);

	if (!composite) printf("Error allocating output frame!\n");
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
	uint16_t* outbytes;
	composite->GetBytes((void**)&outbytes);

	if (stream == 1) {
		frame1 = frame;

		uint16_t* inLeft;
		frame1->GetBytes((void**)&inLeft);

		for (int i = 0; i < composite->GetHeight(); ++i) {
			// do left part
			uint16_t* lstartPtr = inLeft + WIDTH * i + (WIDTH / 4);
			uint16_t* dstPtr = outbytes + WIDTH * i;

			memcpy(dstPtr, lstartPtr, WIDTH);
		}
	}
	else {
		frame2 = frame;

		uint16_t* inRight;
		frame2->GetBytes((void**)&inRight);

		for (int i = 0; i < composite->GetHeight(); ++i) {
			uint16_t* dstRowPtr = outbytes + WIDTH * i;

			// do right part
			uint16_t* rstartPtr = (inRight + WIDTH * i) + WIDTH / 4;
			uint16_t* dstPtr = dstRowPtr + WIDTH / 2;
			memcpy(dstPtr, rstartPtr, WIDTH);

		}
	}

	trigger();

	frame->Release();
}

void VideoProcessor::trigger()
{
	output->showFrame(composite);
}
