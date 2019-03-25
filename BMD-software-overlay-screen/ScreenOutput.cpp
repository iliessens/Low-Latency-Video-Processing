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

	window = SDL_CreateWindow("Video output", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
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

	SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, 
		SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	
	int* videoData;
	int pitch;
	SDL_LockTexture(buffer, NULL, (void**) &videoData, &pitch);

	memcpy(videoData, data, WIDTH*HEIGHT*IMAGE_CHANNELS);

	SDL_UnlockTexture(buffer);

	SDL_RenderCopy(renderer, buffer, NULL, NULL);
	SDL_RenderPresent(renderer);

#if EN_CHRONO
	Chronometer::stopAndPrint();
#endif

	SDL_DestroyTexture(buffer);
	frame->Release();

	return 0;
}

int ScreenOutput::showFrame(IDeckLinkVideoFrame * frame) {
	// do in separate thread
	std::async(std::launch::async,drawAction, frame);

	return 0;
}