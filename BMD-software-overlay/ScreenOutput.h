#pragma once

#include <SDL.h>

class ScreenOutput {
private:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screenSurface;
public:
	ScreenOutput();
	~ScreenOutput();
	int showFrame(IDeckLinkVideoFrame * frame);
};