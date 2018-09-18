#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL.h>
#include "common.h"
#include "game.h"

struct Game;
typedef int TextureId;

typedef struct Texture {
    char    filename[MAX_PATH];
    int     width;
    int     height;

    SDL_Texture* tex;
} Texture;

typedef struct TextureManager {
    SDL_Renderer* renderer;
    Texture** textureArray;
} TextureManager;

TextureManager* textureManagerInit(struct Game* game);
void textureManagerFree(TextureManager* tm);

TextureId textureManagerLoadTexture(TextureManager* tm, const char* filename);
void textureManagerFreeTexture(TextureManager* tm, TextureId texId);

Texture* textureManagerGetTexture(TextureManager* tm, TextureId texId);

#endif
