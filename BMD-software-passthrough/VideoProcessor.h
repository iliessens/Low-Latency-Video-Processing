#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;

public:
	VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame);

	void setOutput(DeckLinkOutputDevice* output);
};