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
	if (stream == 1) {
		t1 = high_resolution_clock::now();
	}
	else {
		high_resolution_clock::time_point t2 = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(t2 - t1).count();

		if (duration > (FRAME_TIME / 2)) {
			triggerStream = 1;
		}
		else {
			triggerStream = 2;
		}
	}

	if (stream == triggerStream) {
		processor->publishFrame(frame, stream);
	}
	else {
		frame->Release();
	}
}

void Framesync::setOutput(DeckLinkOutputDevice* output) {
	AbstractVideoProcessor::setOutput(output); // pass to default setter
	processor->setOutput(output);
}