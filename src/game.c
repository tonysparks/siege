#include "math.h"
#include "game.h"
#include "common.h"
#include "renderer.h"

#include "SDL.h"
#include "SDL_image.h"

#include "texture_manager.h"
#include "sound_manager.h"

static TextureId hero = 0;

static void gameUpdate(Game* game) {

}

static void gameRender(Game* game) {
    SDL_RenderClear(game->renderer);
    Vec2 pos = {20, 20};
    drawTexture(hero, pos);
    SDL_RenderPresent(game->renderer);
}


Game* gameInit(GameConfig* config) {    
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
        logger(FATAL_LEVEL, "Unable to Init SDL: %s\n", SDL_GetError());
        return NULL;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        logger(ERROR_LEVEL, "Unable to Init hinting: %s\n", SDL_GetError());        
    }

    SDL_Window* window = NULL;

    if((window = SDL_CreateWindow(config->title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        config->windowWidth, config->windowHeight, 
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL)) == NULL) {

        logger(FATAL_LEVEL, "Unable to create SDL Window: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Renderer* renderer = NULL;
    if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL) {
        logger(FATAL_LEVEL, "Unable to create renderer: %s\n", SDL_GetError());
        return NULL;
	}

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		logger(FATAL_LEVEL, "Unable to init SDL_image: %s\n", IMG_GetError());
		return NULL;
	}

    if(TTF_Init() == -1) {
        logger(FATAL_LEVEL, "Unable to init TTF_Init: %s\n", TTF_GetError());
		return NULL;
    }


    Game* game = (Game*)siegeMalloc(sizeof(Game));
    game->config = config;
    game->renderer = renderer;
    game->surface = surface;
    game->window = window;
    game->isRunning = 0;
    game->textureManager = textureManagerInit(game);

    soundManagerInit();
    rendererInit(game);
// Test Code

    hero = textureManagerLoadTexture(game->textureManager, "../assets/gfx/german.bmp");
    

  //  SoundId zingId = loadSound("../assets/sfx/bullet_zing01.wav");
   // SoundSource source = playSound(zingId, -1);


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

    rendererFree();
    soundManagerFree();

    TTF_Quit();
    IMG_Quit();
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

        // update
        gameUpdate(game);

        // render
        gameRender(game);
    }
}

