#include "math.h"
#include "game.h"
#include "common.h"
#include "renderer.h"

#include "SDL.h"
#include "SDL_image.h"

#include "console.h"

#include "texture_manager.h"
#include "sound_manager.h"
#include "model.h"

#include "input_system.h"

static TextureId hero = 0;
static FontId fontId  = -1;

static Model fireModel = {
    .animation = {
        .numberOfFrames = 4,
        .elapsedTime = 0,
        .frameTime = 1140,
        .currentFrame = 0,
        .loop = 1,
        .isPaused = 0
    },    
    .numOfFrames = 4,
};

static Game* gGame = NULL;

static void cmdQuit(const char* args) {
    if(gGame) {
        gGame->isRunning = 0;
    }
}

static void gameUpdate(Game* game, TimeStep* timeStep) {
    SDL_Event event;

    while(SDL_PollEvent(&event) != 0) {
        switch(event.type) {
            case SDL_QUIT: {
                game->isRunning = 0;
                break;
            }
            case SDL_KEYDOWN: {
                if(event.key.keysym.sym == SDLK_BACKQUOTE) {
                    consoleToggle();
                    break;
                }
                inputSystemHandleEvent(&event);
                break;
            }
            default: {
                inputSystemHandleEvent(&event);
            }
        }    
    }

    inputSystemUpdate(timeStep);

    consoleUpdate(timeStep);

    modelUpdate(&fireModel, timeStep);

}

static void gameRender(Game* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    Vec2 pos = {20, 120};
    drawTexture(hero, pos);

  //  drawSprite(guy);

    Color color = {
        .r = 1,
        .g = 0,
        .b = 0,
        .a = 1
    };
    VectorSet(pos, 0, 0);
    //drawText(fontId, &color, pos, "Hello '%s'", "there");
    drawText(fontId, &color, pos, "Hello %d %d", fireModel.animation.currentFrame, fireModel.animation.elapsedTime);

    modelDraw(&fireModel);

    consoleDraw();
    
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
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, WORLD_WIDTH, WORLD_HEIGHT);
    //SDL_RenderSetIntegerScale(renderer, 1);

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        logger(FATAL_LEVEL, "Unable to init SDL_image: %s\n", IMG_GetError());
        return NULL;
    }

    if(TTF_Init() == -1) {
        logger(FATAL_LEVEL, "Unable to init TTF_Init: %s\n", TTF_GetError());
        return NULL;
    }

    fileSystemInit(NULL);
    consoleInit();

    Game* game = (Game*)siegeMalloc(sizeof(Game));
    game->config = config;
    game->renderer = renderer;
    game->surface = surface;
    game->window = window;
    game->isRunning = 0;
    game->textureManager = textureManagerInit(game);

    gGame = game;

    soundManagerInit();
    rendererInit(game);
// Test Code

    hero = textureManagerLoadTexture(game->textureManager, "gfx/german_south_fire.png");

    float x = 16;
    float y = 8;

    for(size_t i = 0; i < fireModel.numOfFrames; ++i) {
        SpriteId frame = allocSprite(hero);
        Sprite* sprite = getSprite(frame);
        if(sprite) {
            fireModel.sprites[i] = frame;

            sprite->bounds.x = x;
            sprite->bounds.y = y;
            sprite->bounds.w = 35;
            sprite->bounds.h = 55;

            VectorSet(sprite->pos, 50, 60);        
            VectorSet(sprite->rotationPos, 0, 0);            
            x += 64;            
        }
    }
    
    fontId = loadFont("gfx/fonts/courier_new.ttf", 11);
    //SoundId zingId = loadSound("sfx/bullet_zing01.wav");
    //SoundSource source = playSound(zingId, -1);

    //consolePrintf("This\nis a\ntest\n1\n2\n3\n4\n5\n6\n7\n8\n9\n0\n11\n22\n33\n44\n55\n66\n77\n88\n99\n00\n");
    //consolePrintf("End!\n");

    consoleAddCommand("quit", &cmdQuit);
    
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
    fileSystemFree();

    consoleFree();

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void  gameRun(Game* game) {
    if(game->isRunning) {
        return;
    }

    game->isRunning = 1;

    long currentTime = SDL_GetTicks();
    long accumulator = 0;
    long gameClock = 0;

    int maxIterations = 5;
    long maxDelta = 250;
    long frameRate = 30;
    long dt = 1000 / frameRate;

    TimeStep timeStep = {
        .frameTime = dt,
        .frameTimeFraction = dt / 1000.0,
        .gameClock = gameClock

    };
    
    while(game->isRunning) {
        long newTime = SDL_GetTicks();
        long deltaTime = newTime - currentTime;

        if(deltaTime > maxDelta) {
            deltaTime = maxDelta;
        }

        // Run game logic at a fixed frequency
        if(deltaTime >= dt) {
            currentTime = newTime;

            accumulator += deltaTime;
            int iteration = 0;
            while(accumulator >= dt && iteration < maxIterations) {
                timeStep.gameClock = gameClock;

                gameUpdate(game, &timeStep);

                gameClock   += dt;
                accumulator -= dt;
                iteration++;
            }
        }

        gameRender(game);
    }
}

