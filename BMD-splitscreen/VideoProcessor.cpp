#include "VideoProcessor.h"
#include <assert.h>

#include "Chronometer.h"

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
	uint16_t* inBytes;

	composite->GetBytes((void**)&outbytes);
	frame->GetBytes((void**) &inBytes);

	if (stream == 1) {
		copyData(inBytes + (WIDTH / 4), outbytes);
	}
	else {
		copyData(inBytes + (WIDTH / 4), outbytes + (WIDTH / 2));
	}

	trigger();

	frame->Release();
}

void VideoProcessor::copyData(uint16_t * input, uint16_t * output)
{
	for (int i = 0; i < composite->GetHeight(); ++i) {
		uint16_t* dstPtr = output + WIDTH * i;

		uint16_t* rstartPtr = (input + WIDTH * i);
		memcpy(dstPtr, rstartPtr, WIDTH);

	}
}

void VideoProcessor::trigger()
{
	output->showFrame(composite);
	Chronometer::stopAndPrint();
}
