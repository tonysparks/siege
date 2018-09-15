#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "common.h"
#include "game.h"

void rendererInit(Game* game);
void rendererFree();

void drawTexture(TextureId texId, Vec2 pos);
void drawSubTexture(TextureId texId, Vec2 pos, Rect rect);
#endif
