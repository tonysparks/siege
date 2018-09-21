#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SDL.h>
#include "common.h"

/*
typedef enum KeyEventFlags {
    KEY_RELEASED   = (1<<1),
    KEY_PRESSED    = (1<<2),
    ALT_PRESSED    = (1<<3),
    SHIFT_PRESSED  = (1<<4),
    CTRL_PRESSED   = (1<<5),
} KeyEventFlags;
*/

typedef struct KeyEvent {
    int keyCode;
    SDL_Keymod keymod;
} KeyEvent;

typedef void (*OnKeyEvent)(KeyEvent event);


typedef enum MouseEventFlags {
    BUTTON_RELEASED   = (1<<1),
    BUTTON_PRESSED    = (1<<2),
} MouseEventFlags;

typedef struct MouseEvent {
    int button;
    MouseEventFlags flags;
} MouseEvent;

typedef void (*OnMouseEvent)(MouseEvent event);

void inputSystemInit();
void inputSystemFree();

void inputSystemUpdate(TimeStep* timeStep);

void inputSystemHandleEvent(SDL_Event* event);
void inputSystemGetMousePos(Vec2 mousePos);

void inputSystemRegisterKeyboardHandler(OnKeyEvent onKey);
void inputSystemRegisterMouseHandler(OnMouseEvent onMouse);

#endif
