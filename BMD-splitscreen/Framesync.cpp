#include <thread>

#include "Framesync.h"
#include "settings.h"

Framesync::Framesync() {
	processor = new VideoProcessor();
}

Framesync::~Framesync()
{
	delete processor;
}


void Framesync::publishFrame(IDeckLinkVideoFrame * frame, char stream)
{

	std::thread pt(&VideoProcessor::publishFrame, processor,frame, stream);
	pt.detach();

	// run in separate thread
	std::thread th(&VideoProcessor::trigger, processor);
	th.detach();
}

void Framesync::setOutput(DeckLinkOutputDevice* output) {
	AbstractVideoProcessor::setOutput(output); // pass to default setter
	processor->setOutput(output);
}