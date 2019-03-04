#include <stdio.h>
#include "ImageSource.h"
#include "settings.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageSource::ImageSource() {
	width = 0;

	imgPtr = stbi_load(IMAGE_NAME, &width, &height, &bpp, IMAGE_CHANNELS);

	if(width != 0) printf("Image loaded: %dx%d,  %d channels", width, height, bpp);
}

ImageSource::~ImageSource() {
	stbi_image_free(imgPtr);
}

uint8_t * ImageSource::getImage()
{
	return imgPtr;
}
