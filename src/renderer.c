#include "renderer.h"
#include "texture_manager.h"

static TextureManager* texManager = NULL;
static SDL_Renderer*   renderer   = NULL;

void rendererInit(Game* game) {
    texManager = game->textureManager;
    renderer   = game->renderer;
}

void rendererFree() {
    texManager = NULL;
    renderer   = NULL;
}

void drawTexture(TextureId texId, Vec2 pos) {
    Texture* tex = textureManagerGetTexture(texManager, texId);
    if(tex) {
        SDL_Rect dest = {
            pos[0], pos[1], tex->width, tex->height
        };

        SDL_RenderCopy(renderer, tex->tex, NULL, &dest);
    }
}
void drawSubTexture(TextureId texId, Vec2 pos, Rect rect) {
    Texture* tex = textureManagerGetTexture(texManager, texId);
    if(tex) {
        SDL_Rect src = {
            rect.x, rect.y, rect.w, rect.h
        };

        SDL_Rect dest = {
            pos[0], pos[1], rect.w, rect.h
        };

        SDL_RenderCopy(renderer, tex->tex, &src, &dest);
    }
}