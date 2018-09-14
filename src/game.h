#ifndef GAME_H
#define GAME_H

typedef struct GameConfig {
    int windowWidth;
    int windowHeight;
    char*  title;
} GameConfig;

typedef struct Game {
    int     isRunning;

    GameConfig*     config;
    SDL_Window*     window;
    SDL_Surface*    surface;
    SDL_Renderer*   renderer;
} Game;



Game* gameInit(GameConfig* config);
void  gameFree(Game* game);
void  gameRun(Game* game);

#endif
