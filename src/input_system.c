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
static Vec2         inMousePos;

void inputSystemInit() {

}
void inputSystemFree() {

}

static void inputSystemEmitKeyEvent(SDL_Event* event) {
    KeyEvent keyEvent;
    keyEvent.keyCode = event->key.keysym.sym;
    keyEvent.keymod  = event->key.keysym.mod;
    keyEvent.type    = event->type;

    if(keyEvent.keyCode > -1 && keyEvent.keyCode < 128) {
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

    if(!inNumKeyListeners) {
        return;
    }

    for(size_t i = 0; i <inNumKeyListeners; i++) {
        inKeyEventListeners[i](&keyEvent);
    }
}

static void inputSystemEmitMouseEvent(SDL_Event* event) {
    MouseEvent mouseEvent;
    mouseEvent.type = event->type;

    if(event->type == SDL_MOUSEWHEEL) {
        mouseEvent.button = 0;
        mouseEvent.flags  = 0;        
        mouseEvent.wheelDirection = event->wheel.y;
    }
    else if(event->type == SDL_MOUSEMOTION) {
        VectorSet(inMousePos, event->motion.x, event->motion.y);

        mouseEvent.button = 0;
        mouseEvent.flags  = event->motion.state;
        mouseEvent.wheelDirection = 0;
    }
    else {
        VectorSet(inMousePos, event->button.x, event->button.y);

        mouseEvent.button = event->button.button;
        mouseEvent.flags  = event->button.state;
        mouseEvent.wheelDirection = 0;
    }

    VectorCopy(inMousePos, mouseEvent.pos);


    for(size_t i = 0; i <inNumMouseListeners; i++) {
        inMouseEventListeners[i](&mouseEvent);
    }
}

void inputSystemHandleEvent(SDL_Event* event) {
    switch(event->type) {
        case SDL_KEYDOWN:            
        case SDL_KEYUP:
            inputSystemEmitKeyEvent(event);
            break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
            inputSystemEmitMouseEvent(event);
            break;
    }
}

void inputSystemUpdate(TimeStep* timeStep) {

}

void inputSystemGetMousePos(Vec2 mousePos) {
    VectorCopy(inMousePos, mousePos);
}

void inputSystemRegisterKeyboardHandler(OnKeyEvent onKey) {
    if(inNumKeyListeners + 1 > MAX_LISTENERS) {
        logger(ERROR_LEVEL, "Unable to register Keyboard handler, as the number of registered handlers has exceeded the max: %d \n", MAX_LISTENERS);
        return;
    }

    inKeyEventListeners[inNumKeyListeners++] = onKey;
}


void inputSystemRegisterMouseHandler(OnMouseEvent onMouse) {
    if(inNumMouseListeners + 1 > MAX_LISTENERS) {
        logger(ERROR_LEVEL, "Unable to register Mouse handler, as the number of registered handlers has exceeded the max: %d \n", MAX_LISTENERS);
        return;
    }

    inMouseEventListeners[inNumMouseListeners++] = onMouse;
}