#pragma once
#include <stdint.h>

class ImageSource {
private:
	uint8_t * imgPtr;
	uint8_t*  alphaPtr = NULL;
	int width, height, bpp;

	void fixPixelFormat();

public:
	ImageSource(char* name, int channels);
	~ImageSource();
	uint8_t* getImage();
	uint8_t * getAlpha();

	void premultiply();
	void preCalculate();
};