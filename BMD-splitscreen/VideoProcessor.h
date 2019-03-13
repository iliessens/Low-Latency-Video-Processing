#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

#include <stdint.h>

#include "settings.h"

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;

	bool processFrame(IDeckLinkVideoFrame * frame);


public:
	VideoProcessor();
	~VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame, char stream);

	void setOutput(DeckLinkOutputDevice* output);
};