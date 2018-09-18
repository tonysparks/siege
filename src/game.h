#ifndef GAME_H
#define GAME_H

// 1.333 aspect
//256x192
//480x360
//512x384

// 1.777 aspect
#define WORLD_WIDTH 480
#define WORLD_HEIGHT 270

struct TextureManager;

typedef struct GameConfig {
    int windowWidth;
    int windowHeight;
    char*  title;
} GameConfig;

typedef struct Game {
    int     isRunning;

    struct TextureManager* textureManager;

    GameConfig*     config;
    SDL_Window*     window;
    SDL_Surface*    surface;
    SDL_Renderer*   renderer;
} Game;



Game* gameInit(GameConfig* config);
void  gameFree(Game* game);
void  gameRun(Game* game);

#endif
