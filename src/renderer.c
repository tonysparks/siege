#include "renderer.h"
#include "texture_manager.h"
#include "file_system.h"

#define MAX_TEXT_LENGTH 1024

// TODO: Create VideoDevice structure for all of these...
static TextureManager* texManager = NULL;
static SDL_Renderer*   renderer   = NULL;
static SDL_Surface*    screen     = NULL;
static SDL_Window*     window     = NULL;

static Sprite          sprites[MAX_SPRITES];
static Font            fonts[MAX_FONTS];

void rendererInit(Game* game) {
    if(TTF_Init() < 0) {
        logger(FATAL_LEVEL, "Unable to initialize TTF_Init %s \n", TTF_GetError());
    }

    texManager = game->textureManager;
    renderer   = game->renderer;
    screen     = game->surface;
    window     = game->window;

    for(int i = 0; i < MAX_SPRITES; ++i) {
        Sprite* sprite = &sprites[i];
        sprite->texId = 0;        
    }

    for(int i = 0; i < MAX_FONTS; ++i) {
        Font* font = &fonts[i];
        memset(font->filename, 0, MAX_PATH);
        font->font = NULL;
        font->surface = NULL;
        font->size = 0;
    }
}

void rendererFree() {
    texManager = NULL;
    renderer   = NULL;
    screen     = NULL;
    window     = NULL;

    for(int i = 0; i < MAX_FONTS; ++i) {
        Font* font = &fonts[i];
        if(font->font) {
            freeFont(i);
        }
        memset(font->filename, 0, MAX_PATH);
        font->surface = NULL;
        font->font = NULL;
        font->size = 0;
    }

    TTF_Quit();
}

int rendererGetWidth() {
    int w = 0;
    SDL_GL_GetDrawableSize(window, &w, NULL);

    return w;
}
int rendererGetHeight() {
    int h = 0;
    SDL_GL_GetDrawableSize(window, NULL, &h);

    return h;
}

#define IS_VALID_FONTID(fid) ((fid) > -1 && (fid) < MAX_FONTS)

FontId loadFont(const char* filename, int size) {    
    for(int i = 0; i < MAX_FONTS; ++i) {
        Font* font = &fonts[i];        
        if(!strcmp(filename, font->filename)) {
            if(font->size == size) {
                return i;
            }
        }
        
    }

    for(int i = 0; i < MAX_FONTS; ++i) {
        Font* font = &fonts[i];
        if(!font->font) {
            TTF_Font* f = TTF_OpenFontRW(fileSystemReadFully(filename), 1, size);
            if(!f) {
                logger(ERROR_LEVEL, "Unable to load font '%s' : %s \n", filename, TTF_GetError());
                return -1;
            }
            strcpy(font->filename, filename);
            font->surface = NULL;
            font->font = f;
            font->size = size;
            return i;
        }
    }

    logger(ERROR_LEVEL, "No more free space available for loading font: '%s' \n", filename);

    return -1;
}
void   freeFont(FontId fid) {
    if(IS_VALID_FONTID(fid)) {
        Font* font = &fonts[fid];
        memset(font->filename, 0, MAX_PATH);

        font->size = 0;
        if(font->font) {
            TTF_CloseFont(font->font);
        }
        font->font = NULL;
        if(font->surface) {
            SDL_FreeSurface(font->surface);
        }
        font->surface = NULL;
    }
}

Maybe fontWidthHeight(FontId fid, int* width, int* height, const char* format, ...) {
    static char text[MAX_TEXT_LENGTH];

    if(IS_VALID_FONTID(fid)) {
        Font* font = &fonts[fid];

        va_list args;
        va_start(args, format);
        vsprintf_s(text, MAX_TEXT_LENGTH, format, args);
        va_end(args);   

        if(!TTF_SizeText(font->font, text, width, height)) {
            return OK;
        }
    }

    return FAILED;
}

void drawText(FontId fid, Color* color, Vec2 pos, const char* format, ...) {
    static char text[MAX_TEXT_LENGTH];

    if(IS_VALID_FONTID(fid)) {
        Font* font = &fonts[fid];

        va_list args;
        va_start(args, format);
        vsprintf_s(text, MAX_TEXT_LENGTH, format, args);
        va_end(args);   

        SDL_Color c = {
            .r = color->r,
            .g = color->g,
            .b = color->b,
            .a = color->a,
        };

        SDL_Surface* textSurface = TTF_RenderText_Solid(font->font, text, c);
        if(!textSurface) {
            logger(ERROR_LEVEL, "Failed to render font: %s \n", TTF_GetError());
        }
        else {
            if(font->surface) {
                if(textSurface != font->surface) {
                    SDL_FreeSurface(font->surface);
                }
            }

            font->surface = textSurface;
            
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, textSurface);
            if(tex) {
                SDL_Rect dest = {
                    pos[0], pos[1], textSurface->w, textSurface->h
                };

                SDL_RenderCopy(renderer, tex, NULL, &dest);
                SDL_DestroyTexture(tex);
            }
        }


    }
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


void drawRect(Rect rect, Color* color) {
    SDL_Rect r = {
        .x = rect.x,
        .y = rect.y,
        .w = rect.w,
        .h = rect.h
    };

    if(color) {
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    }
    SDL_RenderDrawRect(renderer, &r);
}


void fillRect(Rect rect, Color* color) {
    SDL_Rect r = {
        .x = rect.x,
        .y = rect.y,
        .w = rect.w,
        .h = rect.h
    };

    if(color) {
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    }
    SDL_RenderFillRect(renderer, &r);
}


void drawLine(Vec2 a, Vec2 b, Color* color) {    
    if(color) {
        SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    }
    SDL_RenderDrawLine(renderer, a[0], a[1], b[0], b[1]);
}

#define IS_VALID_SPRITEID(id) ((id) > -1 && (id) < MAX_SPRITES)

SpriteId allocSprite(TextureId texId) {
    for(int i = 0; i < MAX_SPRITES; ++i) {
        Sprite* sprite = &sprites[i];
        if(sprite->texId == 0) {
            Texture* tex = textureManagerGetTexture(texManager, texId);
            if(tex) {
                sprite->texId = texId;
                sprite->flags = 0;
                sprite->rotation = 0;

                rectClear(sprite->bounds);
                sprite->bounds.w = tex->width;
                sprite->bounds.h = tex->height;

                VectorClear(sprite->pos);                
                VectorSet(sprite->rotationPos, tex->width/2, tex->height/2);                
                sprite->color.r = sprite->color.g = sprite->color.b = sprite->color.a = 255;

                return i;
            }
        }
    }

    return -1;
}
void    freeSprite(SpriteId sid) {
    if(IS_VALID_SPRITEID(sid)) {
        Sprite* sprite = &sprites[sid];
        sprite->texId = 0;
    }
}

Sprite* getSprite(SpriteId sid) {
    if(IS_VALID_SPRITEID(sid)) {
        return &sprites[sid];
    }

    return NULL;
}

void drawSprite(SpriteId sid) {
    Sprite* sprite = getSprite(sid);
    if(sprite) {
        Texture* tex = textureManagerGetTexture(texManager, sprite->texId);
        if(tex) {
            SDL_Rect src = {
                sprite->bounds.x, sprite->bounds.y, sprite->bounds.w, sprite->bounds.h
            };

            SDL_Rect dest = {
                sprite->pos[0], sprite->pos[1], sprite->bounds.w, sprite->bounds.h
            };

            SDL_Point center = {
                .x = sprite->rotationPos[0],
                .y = sprite->rotationPos[1],
            };

            SDL_RendererFlip flip = SDL_FLIP_NONE;
            if(sprite->flags & FLIP_X) {
                flip |= SDL_FLIP_HORIZONTAL;
            }

            if(sprite->flags & FLIP_Y) {
                flip |= SDL_FLIP_VERTICAL;
            }

            SDL_RenderCopyEx(renderer,
                             tex->tex,
                             &src,
                             &dest,
                             sprite->rotation,
                             &center,
                             flip);
        }
    }
}