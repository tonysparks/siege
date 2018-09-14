#include "renderer.h"


Maybe textureLoad(Game* game, Texture* texture, const char* filename) {
    SDL_Texture* tex = IMG_LoadTexture(game->renderer, filename);
    if(!tex) {
        logger(ERROR_LEVEL, "Failed to load texture '%s' : %s", filename, IMG_GetError());
        return FAILED;
    }

    texture->tex = tex;
    SDL_QueryTexture(tex, NULL, NULL, &texture->width, &texture->height);
    return OK;
}

void textureFree(Texture* texture) {
    if(texture) {
        if(texture->tex) {
            SDL_DestroyTexture(texture->tex);
            texture->tex = NULL;            
        }
        texture->width = 0;
        texture->height = 0;
    }
}