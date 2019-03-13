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

public:
	VideoProcessor();
	~VideoProcessor();

	void publishFrame(IDeckLinkVideoFrame* frame, char stream);
};