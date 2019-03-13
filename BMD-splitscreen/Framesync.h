#pragma once
#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

#include <stdint.h>

#include "settings.h"
#include "AbstractVideoProcessor.h"
#include "VideoProcessor.h"

class Framesync : public AbstractVideoProcessor {
private:
	DeckLinkOutputDevice* output;
	VideoProcessor* processor;

public:
	Framesync();
	~Framesync();

	void publishFrame(IDeckLinkVideoFrame* frame, char stream);
	void setOutput(DeckLinkOutputDevice* output);
};