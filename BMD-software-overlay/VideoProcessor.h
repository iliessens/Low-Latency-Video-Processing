#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"
#include <stdint.h>

#include "ImageSource.h"

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;
	ImageSource* imageSource;
	uint8_t* overlayPtr;
	uint8_t* alphaPtr;

	inline void processFrame(IDeckLinkVideoFrame * frame);

public:
	VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame);

	void setOutput(DeckLinkOutputDevice* output);
};