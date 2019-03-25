#include <stdio.h>
#include <stdint.h>
#include <future>

#include "DeckLinkAPI_h.h"

#include "ScreenOutput.h"
#include "settings.h"
#include "Chronometer.h"

SDL_Renderer * renderer;

// masks for bit array
uint32_t rmask, gmask, bmask, amask;

ScreenOutput::ScreenOutput() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	window = SDL_CreateWindow("Video output", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH/4, HEIGHT/4, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


	rmask = 0x00ff0000;
	gmask = 0x0000ff00;
	bmask = 0x000000ff;
	amask = 0xff000000;
}

ScreenOutput::~ScreenOutput() {
	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();
}

int drawAction(IDeckLinkVideoFrame * frame) {
	uint8_t* data;
	frame->GetBytes((void**)&data);

	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, WIDTH, HEIGHT, IMAGE_CHANNELS * 8, IMAGE_CHANNELS * WIDTH,
		rmask, gmask, bmask, amask);

	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

#if EN_CHRONO
	Chronometer::stopAndPrint();
#endif

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);
	frame->Release();

	return 0;
}

int ScreenOutput::showFrame(IDeckLinkVideoFrame * frame) {
	// do in separate thread
	std::async(std::launch::async,drawAction, frame);

	return 0;
}