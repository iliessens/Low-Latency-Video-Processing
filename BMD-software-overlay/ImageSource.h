#pragma once
#include <stdint.h>

class ImageSource {
private:
	uint8_t * imgPtr;
	int width, height, bpp;

public:
	ImageSource();
	~ImageSource();
	uint8_t* getImage();
};