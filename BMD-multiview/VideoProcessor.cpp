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

	// make frame black
	uint8_t* data;
	composite->GetBytes((void**) &data);
	int frameSize = composite->GetRowBytes() * composite->GetHeight();
	// lopp over two pixels at a time (4:2:2 format)
	for (long i = 0; i < frameSize; i= i + 4) {
		data[i + 3] = 16; // Y1
		data[i + 2] = 128; // Cb
		data[i + 1] = 16; // Y0
		data[i + 0] = 128; // Cr
	}

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
	uint8_t* outbytes;
	composite->GetBytes((void**)&outbytes);

	//if (stream == 1) {
	if (false) {
		frame1 = frame;

		uint8_t* inLeft;
		frame1->GetBytes((void**)&inLeft);

		for (int i = 0; i < composite->GetRowBytes(); i = i + 2) { // row loop, copy 1/2 of rows
			int v = i / 2;

			// do left part
			uint8_t* lstartPtr = inLeft + WIDTH * v + (WIDTH / 4);
			uint8_t* dstPtr = outbytes + WIDTH * v;

			for (int j = 0; j < composite->GetWidth() / 2; j = j + 2) {
				dstPtr[j] = lstartPtr[j];
			}
		}
	}
	else {
		frame2 = frame;

		uint8_t* inRight;
		frame2->GetBytes((void**)&inRight);
		long rowbytes = composite->GetRowBytes();

		for (int i = 0; i < composite->GetHeight()/2; i++) { // row loop

			// do right part
			uint8_t* rstartPtr = inRight + rowbytes * i * 2;
			uint8_t* dstPtr = outbytes + rowbytes * i + rowbytes / 2 + rowbytes * composite->GetHeight() / 4;

			for (int j = 0; j < composite->GetWidth(); j = j + 4) {
				dstPtr[j] = rstartPtr[j*2];
				dstPtr[j+1] = rstartPtr[j * 2+1];
				dstPtr[j+2] = rstartPtr[j * 2+2];
				dstPtr[j+3] = rstartPtr[j * 2+3];
			}

		}
	}

	trigger();

	frame->Release();
}

void VideoProcessor::trigger()
{
	output->showFrame(composite);
	Chronometer::stopAndPrint();
}
