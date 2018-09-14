#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "common.h"
#include "game.h"

typedef struct Texture {
    int     width;
    int     height;

    SDL_Texture* tex;
} Texture;

Maybe textureLoad(Game* game, Texture* texture, const char* filename);
void  textureFree(Texture* texture);

#endif
