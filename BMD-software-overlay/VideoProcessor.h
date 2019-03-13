#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

#include <stdint.h>
#include <chrono>
#include <thread> //std::thread

#include "settings.h"
#include "ImageSource.h"

class VideoProcessor {
private:
	DeckLinkOutputDevice* output;
	ImageSource* imageSource;
	uint8_t* overlayPtr;
	uint8_t* alphaPtr;
	std::chrono::milliseconds* timeout;

	void inputWatchdog();
	std::thread* inputWacthdogThread;

	inline bool processFrame(IDeckLinkVideoFrame * frame);


public:
	VideoProcessor();
	~VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame);

	void setOutput(DeckLinkOutputDevice* output);
};