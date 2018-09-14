#include "game.h"
#include "common.h"

#include "SDL.h"

Game* gameInit(GameConfig* config) {    
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		logger(FATAL_LEVEL, "Unable to Init SDL: %s", SDL_GetError());
		return NULL;
	}

	if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		logger(FATAL_LEVEL, "Unable to Init hinting: %s", SDL_GetError());
        return NULL;
	}

    SDL_Window* window = NULL;

	if((window = SDL_CreateWindow(config->title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		config->windowWidth, config->windowHeight, SDL_WINDOW_SHOWN)) == NULL) {

		logger(FATAL_LEVEL, "Unable to create SDL Window: %s", SDL_GetError());
		return NULL;
	}

	SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Renderer* renderer = NULL;
	if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
	    logger(FATAL_LEVEL, "Unable to create renderer");
	    return NULL;
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    Game* game = (Game*)siegeMalloc(sizeof(Game));
    game->config = config;
    game->renderer = renderer;
    game->surface = surface;
    game->window = window;
    game->isRunning = 0;

    return game;
}

void  gameFree(Game* game) {
    if(game) {
        if(game->renderer) {
            SDL_DestroyRenderer(game->renderer);
        }

        if(game->window) {
            SDL_DestroyWindow(game->window);
        }

        siegeFree(game);
    }

    SDL_Quit();
}
void  gameRun(Game* game) {
    if(game->isRunning) {
        return;
    }

    game->isRunning = 1;

    SDL_Event event;
    while(game->isRunning) {
        while(SDL_PollEvent(&event) != 0) {
            switch(event.type) {
                case SDL_QUIT: {
                    game->isRunning = 0;
                    break;
                }
            }
        }
    }
}