#pragma once

#include <SDL.h>

class ScreenOutput {
private:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;

	SDL_Renderer * renderer;

	// masks for bit array
	uint32_t rmask, gmask, bmask, amask;
public:
	ScreenOutput();
	~ScreenOutput();
	int showFrame(IDeckLinkVideoFrame * frame);
};