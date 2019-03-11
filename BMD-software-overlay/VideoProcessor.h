#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"
#include <stdint.h>
#include <chrono>
#include "settings.h"

#include "ImageSource.h"

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;
	ImageSource* imageSource;
	uint8_t* overlayPtr;
	uint8_t* alphaPtr;
	std::chrono::milliseconds* timeout;

	inline bool processFrame(IDeckLinkVideoFrame * frame);

public:
	VideoProcessor();
	~VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame);

	void setOutput(DeckLinkOutputDevice* output);
};