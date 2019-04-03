#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

class AbstractVideoProcessor {
protected:
	DeckLinkOutputDevice* output;

public:
	AbstractVideoProcessor();
	virtual ~AbstractVideoProcessor() = 0;
	virtual void publishFrame(IDeckLinkVideoFrame* frame, char stream) = 0;

	virtual void setOutput(DeckLinkOutputDevice* output);
};