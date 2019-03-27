#pragma once
#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

#include <stdio.h>
#include <chrono>
#include <atomic>

#include "settings.h"
#include "AbstractVideoProcessor.h"
#include "VideoProcessor.h"

using namespace std;
using namespace std::chrono;

class Framesync : public AbstractVideoProcessor {
private:
	DeckLinkOutputDevice* output;
	VideoProcessor* processor;

	high_resolution_clock::time_point t1;
	int triggerStream;

public:
	Framesync();
	~Framesync();

	void publishFrame(IDeckLinkVideoFrame* frame, char stream);
	void setOutput(DeckLinkOutputDevice* output);
};