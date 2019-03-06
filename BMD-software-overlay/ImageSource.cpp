#include <stdio.h>
#include "ImageSource.h"
#include "settings.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageSource::ImageSource() {
	imgPtr = stbi_load(IMAGE_NAME, &width, &height, &bpp, IMAGE_CHANNELS);

	if(imgPtr != NULL) printf("Image loaded: %dx%d,  %d channels\n", width, height, bpp);
	else printf("Error loading image!");

	premultiply(imgPtr);
}

ImageSource::~ImageSource() {
	stbi_image_free(imgPtr);
}

uint8_t * ImageSource::getImage()
{
	return imgPtr;
}

void ImageSource::premultiply(uint8_t * inputImg) {
	printf("Premultiplying alpha... ");

	long size = width * height * IMAGE_CHANNELS;
	for (int i = 0; i < size; i = i + 4) { // for all pixels
		uint8_t* pixelPtr = inputImg + i;
		uint8_t alpha =  *(pixelPtr + 3);

		for (int j = 0; j < IMAGE_CHANNELS - 1; j++) {
			//multiply
			uint16_t temp = *(pixelPtr + j)* alpha;
			*(pixelPtr + j) = temp >> 8; // divide by 255
		}
	}

	printf("Done\n");
}
