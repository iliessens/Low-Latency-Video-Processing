#include "Framesync.h"

Framesync::Framesync() {

}

Framesync::~Framesync()
{

}


void Framesync::publishFrame(IDeckLinkVideoFrame * frame, char stream)
{
	if (stream == 1) {
		output->showFrame(frame);
	}
	else {
		IDeckLinkMutableVideoFrame* rgbframe;
		output->getEmptyFrame(&rgbframe);

		output->showFrame(rgbframe);
	}
}

void Framesync::setOutput(DeckLinkOutputDevice * output)
{
	this->output = output;
}