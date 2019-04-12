#pragma once

#include "DeckLinkOutputDevice.h"
#include "DeckLinkAPI_h.h"

#include <stdint.h>

#include "AbstractVideoProcessor.h"
#include "settings.h"

class VideoProcessor : public AbstractVideoProcessor{
private:
	bool processFrame(IDeckLinkVideoFrame * frame);
	IDeckLinkVideoConversion* converter;

	IDeckLinkVideoFrame* frame1 = NULL;
	IDeckLinkVideoFrame* frame2 = NULL;

	IDeckLinkMutableVideoFrame* composite;

	void copyData(uint16_t* input, uint16_t* output);


public:
	VideoProcessor();
	void setOutput(DeckLinkOutputDevice * output);
	~VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame, char stream);
	void trigger();
};