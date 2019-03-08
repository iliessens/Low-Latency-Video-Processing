#pragma once
#include <stdint.h>

class ImageSource {
private:
	uint8_t * imgPtr;
	uint8_t*  alphaPtr;
	int width, height, bpp;
	void premultiply(uint8_t * inputImg);

	void preCalculate();

public:
	ImageSource();
	~ImageSource();
	uint8_t* getImage();
	uint8_t * getAlpha();
};