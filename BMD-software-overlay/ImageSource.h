#pragma once
#include <stdint.h>

class ImageSource {
private:
	uint32_t * imgPtr;
	int width, height, bpp;

public:
	ImageSource();
	~ImageSource();
	uint32_t* getImage();
};