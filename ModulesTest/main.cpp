#include <iostream>
#include <SDL.h>
#include "../TANK/TANKS.h"

int main(int argc, char *argv[]) 
{
	SDL_Init(SDL_INIT_EVERYTHING);

	auto window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{

		case SDL_QUIT:
			quit = true;
		default:
			break;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}