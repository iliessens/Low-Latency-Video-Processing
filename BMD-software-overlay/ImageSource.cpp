#include <stdio.h>
#include "ImageSource.h"
#include "settings.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageSource::ImageSource() {
	uint8_t* input = stbi_load(IMAGE_NAME, &width, &height, &bpp, IMAGE_CHANNELS);
	imgPtr = (uint32_t *) input;

	if(input != NULL) printf("Image loaded: %dx%d,  %d channels\n", width, height, bpp);
	else printf("Error loading image!");
}

ImageSource::~ImageSource() {
	stbi_image_free(imgPtr);
}

uint32_t * ImageSource::getImage()
{
	return imgPtr;
}
