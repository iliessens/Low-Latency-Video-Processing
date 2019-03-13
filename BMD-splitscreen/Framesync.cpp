#include "Framesync.h"

Framesync::Framesync() {
	processor = new VideoProcessor();
}

Framesync::~Framesync()
{
	delete processor;
}


void Framesync::publishFrame(IDeckLinkVideoFrame * frame, char stream)
{
	processor->publishFrame(frame, stream);
}

void Framesync::setOutput(DeckLinkOutputDevice* output) {
	AbstractVideoProcessor::setOutput(output); // pass to default setter
	processor->setOutput(output);
}