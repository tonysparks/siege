#define _CRT_SECURE_NO_WARNINGS

// standard includes
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <Windows.h>

#include <SDL.h>
#include "SDL_ttf.h"

// #include "common.c"
// #include "file_system.c"
// #include "texture_manager.c"
// #include "renderer.c"
// #include "input_system.c"
// #include "console.c"
// #include "sound_manager.c"
// #include "animation.c"
// #include "model.c"
// #include "world.c"
// #include "game.c"

#include "common.h"
#include "file_system.h"
#include "texture_manager.h"
#include "renderer.h"
#include "input_system.h"
#include "console.h"
#include "sound_manager.h"
#include "animation.h"
#include "model.h"
#include "world.h"
#include "game.h"


int main(int argv, char** args) {
    GameConfig config = {
        // 640x480 native resolution (1.33)
        .windowWidth = 640,
        .windowHeight = 360,
        .title = "Logistic Siege"
    };
    
    Game* game = gameInit(&config);
    
    gameRun(game);
    gameFree(game);
    return 0;

}
