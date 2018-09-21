#include "input_system.h"

typedef struct Key {
    long timePressed;
} Key;

#define MAX_LISTENERS 32
#define MAX_KEYS 256

static OnKeyEvent   inKeyEventListeners[MAX_LISTENERS];
static size_t       inNumKeyListeners = 0;

static OnMouseEvent inMouseEventListeners[MAX_LISTENERS];
static size_t       inNumMouseListeners = 0;

static Key          inKeys[MAX_KEYS];


void inputSystemInit() {

}
void inputSystemFree() {

}

void inputSystemEmitKeyEvent(SDL_Event* event) {
    if(!inNumKeyListeners) {
        return;
    }

    KeyEvent keyEvent;
    keyEvent.keyCode = event->key.keysym.sym;
    keyEvent.keymod  = event->key.keysym.mod;

    Key key = inKeys[keyEvent.keyCode];
    if(event->type == SDL_KEYDOWN) {
        if(key.timePressed < 0) {
            key.timePressed = event->key.timestamp;
        }
    }
    else {
        key.timePressed = -1;
    }
}

void inputSystemHandleEvent(SDL_Event* event) {
    switch(event->type) {
        case SDL_KEYDOWN:
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_MOUSEBUTTONUP:
            break;
        case SDL_MOUSEWHEEL:
            break;
    }
}

void inputSystemUpdate(TimeStep* timeStep) {

}

void inputSystemGetMousePos(Vec2 mousePos) {

}

void inputSystemRegisterKeyboardHandler(OnKeyEvent onKey) {

}


void inputSystemRegisterMouseHandler(OnMouseEvent onMouse) {

}