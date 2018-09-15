#include "texture_manager.h"
#include "SDL_image.h"

static Maybe textureLoad(SDL_Renderer* renderer, Texture* texture, const char* filename) {
    SDL_Texture* tex = IMG_LoadTexture(renderer, filename);
    if(!tex) {
        logger(ERROR_LEVEL, "Failed to load texture '%s' : %s", filename, IMG_GetError());
        return FAILED;
    }

    texture->tex = tex;
    texture->filename = filename;

    SDL_QueryTexture(tex, NULL, NULL, &texture->width, &texture->height);
    return OK;
}

static void textureFree(Texture* texture) {
    if(texture) {
        if(texture->tex) {
            SDL_DestroyTexture(texture->tex);
            texture->tex = NULL;            
        }
        texture->width = 0;
        texture->height = 0;
        texture->filename = NULL;
    }
}


TextureManager* textureManagerInit(struct Game* game) {
    TextureManager* tm = (TextureManager*) siegeMalloc(sizeof(TextureManager));
    tm->renderer = game->renderer;
    tm->textureArray = NULL;

    return tm;
}

void textureManagerFree(TextureManager* tm) {
    if(tm) {
        tm->renderer = NULL;
        for(int i = 0; i < stb_sb_count(tm->textureArray); ++i) {
            Texture* tex = tm->textureArray[i];
            if(tex) {
                textureFree(tex);                    
                siegeFree(tex);
            }
        }

        stb_sb_free(tm->textureArray);

        siegeFree(tm);
    }
}

TextureId textureManagerLoadTexture(TextureManager* tm, const char* filename) {
    // first see if this texture already exists
    for(int i = 0; i < stb_sb_count(tm->textureArray); ++i) {
        Texture* tex = tm->textureArray[i];
        if(tex) {
            if(!strcmp(tex->filename, filename)) {
                return (TextureId) i + 1;
            }
        }
    }
    

    // otherwise find a free slot, if no free slot create a new one
    TextureId texId = 0;
    Texture* texture = NULL;

    for(int i = 0; i < stb_sb_count(tm->textureArray); ++i) {
        Texture* tex = tm->textureArray[i];
        if(tex) {
            if(!tex->tex) {
                texture = tex;
                texId = i + 1;
                break;
            }
        }
    }

    if(!texture) {
        texture = (Texture*) siegeMalloc(sizeof(Texture));
        stb_sb_push(tm->textureArray, texture);
        texId = stb_sb_count(tm->textureArray);
    }

    if(textureLoad(tm->renderer, texture, filename) == OK) {    
        return texId;        
    }

    return 0; // invalid texture ID
}


void textureManagerFreeTexture(TextureManager* tm, TextureId texId) {
    if(texId == 0) {
        return;
    }

    int index = texId - 1;
    if(index < stb_sb_count(tm->textureArray) && index > -1) {
        Texture* tex = tm->textureArray[index];
        if(tex) {
            textureFree(tex);
        }
    }
}

Texture* textureManagerGetTexture(TextureManager* tm, TextureId texId) {
    if(texId == 0) {
        return NULL;
    }

    int index = texId - 1;
    if(index < stb_sb_count(tm->textureArray) && index > -1) {
        Texture* tex = tm->textureArray[index];
        if(tex) {
            return tex;
        }
    }

    return NULL;
}