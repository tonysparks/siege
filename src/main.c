#define _CRT_SECURE_NO_WARNINGS

// standard includes
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <Windows.h>

#include <SDL.h>

#include "common.c"
#include "game.c"
#include "renderer.c"

int main(int argv, char** args) {
    GameConfig config = {
        .windowWidth = 600,
        .windowHeight = 480,
        .title = "Logistic Siege"
    };
    
    Game* game = gameInit(&config);
    
    gameRun(game);
    gameFree(game);
    return 0;

}
