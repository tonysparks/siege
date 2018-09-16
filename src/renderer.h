#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "common.h"
#include "game.h"

typedef struct Color {
    union {
        struct {
            float r,g,b,a;
        };
        float v[4];
    };
} Color;

#define colorClear(c) ((c).r = (c).g = (c).b = (c).a = 0)

typedef enum SpriteFlags {
    FLIP_X,
    FLIP_Y,    
} SpriteFlags;

typedef struct Sprite {
    TextureId texId;
    double    rotation;         /* degrees */
    Vec2      rotationPos;      /* rotate around pos */
    Vec2      pos;
    Color     color;
    Rect      bounds;           /* the subimage bounds of the texture */
    SpriteFlags flags;
} Sprite;

#define MAX_SPRITES 1024
typedef int SpriteId;

typedef struct Font {
    const char* filename;
    int         size;
    TTF_Font*   font;
    SDL_Surface* surface;
} Font;

#define MAX_FONTS 12
typedef int FontId;

void rendererInit(Game* game);
void rendererFree();

FontId loadFont(const char* filename, int size);
void   freeFont(FontId fid);

void drawText(FontId fid, Color* color, Vec2 pos, const char* format, ...);

void drawTexture(TextureId texId, Vec2 pos);
void drawSubTexture(TextureId texId, Vec2 pos, Rect rect);


SpriteId allocSprite(TextureId texId);
void     freeSprite(SpriteId sid);
Sprite*  getSprite(SpriteId sid);
void drawSprite(SpriteId sid);


#endif
