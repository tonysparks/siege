#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SDL.h>
#include "common.h"

typedef struct KeyEvent {
    int type;
    int keyCode;
    SDL_Keymod keymod;
} KeyEvent;

typedef void (*OnKeyEvent)(KeyEvent* event);

typedef struct MouseEvent {
    int  type;
    int  button;
    Vec2 pos;
    int  flags;
    int  wheelDirection;
} MouseEvent;

typedef void (*OnMouseEvent)(MouseEvent* event);

void inputSystemInit();
void inputSystemFree();

void inputSystemUpdate(TimeStep* timeStep);

void inputSystemHandleEvent(SDL_Event* event);
void inputSystemGetMousePos(Vec2 mousePos);

void inputSystemRegisterKeyboardHandler(OnKeyEvent onKey);
void inputSystemRegisterMouseHandler(OnMouseEvent onMouse);

#endif
