#include <stdio.h>
#include "ImageSource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageSource::ImageSource(char* name, int channels) {
	imgPtr = stbi_load(name, &width, &height, &bpp, channels);

	if(imgPtr != NULL) printf("Image loaded: %dx%d,  %d channels\n", width, height, bpp);
	else printf("Error loading image!");
}

ImageSource::~ImageSource() {
	stbi_image_free(imgPtr);
	
	if(alphaPtr != NULL) free(alphaPtr);
}

uint8_t * ImageSource::getImage()
{
	return imgPtr;
}

uint8_t* ImageSource::getAlpha() {
	return alphaPtr;
}

void ImageSource::premultiply() {
	printf("Premultiplying alpha... ");

	long size = width * height * bpp;
	for (int i = 0; i < size; i = i + 4) { // for all pixels
		uint8_t* pixelPtr = imgPtr + i;
		uint8_t alpha =  *(pixelPtr + 3);

		for (int j = 0; j < bpp - 1; j++) {
			//multiply
			uint16_t temp = *(pixelPtr + j)* alpha;
			*(pixelPtr + j) = temp >> 8; // divide by 255
		}
	}

	printf("Done\n");
}

// reken op voorhand reeds de 1-alpha waarden uit
void ImageSource::preCalculate() {
	const int resolution = width * height;
	alphaPtr = (uint8_t*) malloc(resolution*4);

	for (int i = 0; i < resolution; ++i) { // for each pixel
		uint8_t pix = 255 - *(imgPtr + 4 * i + 3);
		for (int j = 0; j < 4; ++j) {
			*(alphaPtr + j + 4 * i) = pix;
		}
	}
	}