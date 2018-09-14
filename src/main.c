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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    
    GameConfig config = {
        .windowWidth = 600,
        .windowHeight = 480,
        .title = "Logistic Siege"
    };

    OutputDebugStringA("Hello World");
    Game* game = gameInit(&config);
    
    gameRun(game);
    gameFree(game);
    return 0;

}
