#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"
#include <stdint.h>

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;

public:
	VideoProcessor();

	void processRow(uint32_t* data, int width);

	void publishFrame(IDeckLinkVideoFrame* frame);

	void setOutput(DeckLinkOutputDevice* output);
};