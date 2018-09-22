#include "console.h"
#include "renderer.h"

#include "input_system.h"

#define MAX_INPUT_BUFFER 1024
#define MAX_TEXT_LINES 256
#define MAX_COMMAND_NAME 128

typedef struct Commands {
    Command  cmd;
    char     name[MAX_COMMAND_NAME];
    struct Commands* next;
} Commands;

typedef struct TextLine {
    char line[MAX_INPUT_BUFFER];
    struct TextLine* next;
} TextLine;

typedef struct TextBuffer {
    TextLine*  start;
    TextLine*  end;
    
    TextLine  text[MAX_TEXT_LINES];
    size_t    numberOfLines;
} TextBuffer;

typedef enum ConsoleState {
    OPENING,
    CLOSING,
    NONE,
} ConsoleState;

static TextBuffer cmdTextBuffer;
static char cmdInputBuffer[MAX_INPUT_BUFFER];
static Commands* cmdCommands = NULL;
static long cmdWaitTime = 0;
static int cmdIsActive = 0;
static ConsoleState cmdState = NONE;
static FontId cmdFontId = -1;
static int cmdFontHeight = 0;

static int cmdScrollHeight = 0;
static int cmdHeight = 0;
#define CMD_MAX_HEIGHT 100

void consoleOnKeyEvent(KeyEvent* event) {

}

void consoleOnMouseEvent(MouseEvent* event) {
    //event->
    if(event->type == SDL_MOUSEWHEEL) {
        if(event->wheelDirection > 0) {
            cmdScrollHeight++;

            int numLinesVisible = cmdHeight / cmdFontHeight;
            int max = MAX(cmdTextBuffer.numberOfLines - numLinesVisible, 0);
            if(cmdScrollHeight > max) {
                cmdScrollHeight = max;
            }
            
        }
        else if(event->wheelDirection < 0) {
            cmdScrollHeight--;
            if(cmdScrollHeight < 0) {
                cmdScrollHeight = 0;
            }
        }

        //printf("Scroll Height %d \n", cmdScrollHeight);
    }
}

void consoleInit() {    
    cmdTextBuffer.numberOfLines = 0;    
    cmdTextBuffer.start = cmdTextBuffer.end = &cmdTextBuffer.text[0];
    for(int i = 0; i < MAX_TEXT_LINES; ++i) {
        if(i < MAX_TEXT_LINES - 1) {
            cmdTextBuffer.text[i].next = &cmdTextBuffer.text[i+1];
        }
        else {
            cmdTextBuffer.text[i].next = cmdTextBuffer.start;
        }
        memset(cmdTextBuffer.text[i].line, 0, MAX_INPUT_BUFFER);
    }

    memset(cmdInputBuffer, 0, MAX_INPUT_BUFFER);
    cmdCommands = NULL;

    cmdFontId = loadFont("gfx/fonts/courier_new.ttf", 11);
    if(cmdFontId < 0) {
        logger(FATAL_LEVEL, "Unable to construct console without font!\n");
    }

    int width = 0;
    int height = 0;
    fontWidthHeight(cmdFontId, &width, &height, "W");
    cmdFontHeight = height;

    inputSystemRegisterKeyboardHandler(&consoleOnKeyEvent);
    inputSystemRegisterMouseHandler(&consoleOnMouseEvent);

    // temp
    cmdHeight = CMD_MAX_HEIGHT;
    cmdScrollHeight = 2;
}

void consoleFree() {
    Commands* c = cmdCommands;
    while(c) {
        Commands* next = c->next;
        siegeFree(c);

        c = next;
    }
}



void consolePrintf(const char* format, ...) {
#define CHECK_END()                                        \
    do {                                                   \
      if(cmdTextBuffer.end->next == cmdTextBuffer.start) { \
        cmdTextBuffer.start = cmdTextBuffer.start->next;   \
      }                                                    \
    } while(0)
    CHECK_END();

    va_list args;
    va_start(args, format);
    vsprintf_s(cmdTextBuffer.end->line, MAX_INPUT_BUFFER, format, args);    
    va_end(args);

    TextLine* newLine = cmdTextBuffer.end;
    char* line = cmdTextBuffer.end->line;
    char* start = line;
    size_t n = 0;
    
    while(*line) {
        char c = *line++;
        n++;

        if(c == '\n') {
            cmdTextBuffer.numberOfLines++;

            if(newLine == cmdTextBuffer.end) {
                cmdTextBuffer.end->line[n - 1] = '\0';    
            }
            else {
                strncpy(cmdTextBuffer.end->line, start, n);
                cmdTextBuffer.end->line[n - 1] = '\0';
            }

            cmdTextBuffer.end = cmdTextBuffer.end->next;
            CHECK_END();

            start = line;
            n = 0;
        }
    }

#undef CHECK_END
}
void consoleExecute(const char* format, ...) {

}

void consoleAddCommand(const char* name, Command cmd) {
    Commands* c = cmdCommands;
    Commands* prev = cmdCommands;
    while(c) {
        if(!strcmp(c->name, name)) {
            logger(INFO_LEVEL, "Command '%s' is already bound. \n", name);
            return;
        }

        prev = c;
        c = c->next;
    }

    Commands* newCmd = (Commands*)siegeMalloc(sizeof(Commands));
    newCmd->cmd = cmd;
    memset(newCmd->name, 0, MAX_COMMAND_NAME);
    strncpy(newCmd->name, name, MAX_COMMAND_NAME);

    if(!prev) {
        cmdCommands = newCmd;
    }
    else {
        prev->next = newCmd;
    }
}

void consoleRemoveCommand(const char* name) {
    Commands* c = cmdCommands;
    while(c) {
        if(!strcmp(c->name, name)) {
            Commands* rmCmd = c;
            if(rmCmd == cmdCommands) {
                cmdCommands = c->next;
            }
            else {
                c = c->next;
            }

            siegeFree(rmCmd);
            break;
        }
        
        c = c->next;
    }
}

void consoleToggle() {
    if(!cmdIsActive) {
        cmdState = OPENING;
        cmdIsActive = 1;
    }
    else {
        cmdState = CLOSING;
        cmdIsActive = 0;
    }
}

void consoleDeactivate() {
    cmdState = CLOSING;
    cmdIsActive = 0;
}

int  consoleIsActive() {
    return cmdIsActive;
}

void consoleInputs() {

}

void consoleUpdate(TimeStep* timeStep) {

}


void consoleDraw() {
    static char line[MAX_INPUT_BUFFER];

    if(!cmdFontHeight) {
        return;
    }

    Color fontColor = {
        .r = 255,
        .g = 0,
        .b = 0,
        .a = 255
    };

    Color bgColor = {
        .r = 255,
        .g = 255,
        .b = 255,
        .a = 80
    };

    Color inputBgColor = {
        .r = 205,
        .g = 205,
        .b = 205,
        .a = 120
    };

    Color outlineColor = {
        .r = 0,
        .g = 0,
        .b = 0,
        .a = 255
    };

    Rect background = {
        .x = 0,
        .y = 0,
        .w = rendererGetWidth(),
        .h = cmdHeight + cmdFontHeight
    };

    Rect input = {
        .x = 0,
        .y = background.h,
        .w = rendererGetWidth(),
        .h = cmdFontHeight + 10
    };

    fillRect(background, &bgColor);
    fillRect(input, &inputBgColor);

    Vec2 a;
    VectorSet(a, 0, input.y);

    Vec2 b;
    VectorSet(b, background.w, input.y);

    drawLine(a, b, &outlineColor);

    Vec2 pos;
    VectorSet(pos, 5, 10);

    TextLine* start = cmdTextBuffer.start;
    TextLine* end   = cmdTextBuffer.end;

    int numLinesVisible = cmdHeight / cmdFontHeight;
    int lineNumberStartRendering = MAX(cmdTextBuffer.numberOfLines - (cmdScrollHeight + numLinesVisible), 0);

    int currentLineNumber = 0;
    TextLine* current = start;
    while(current != end) {  

        if(currentLineNumber < lineNumberStartRendering) {
            goto finish;
        }

        if(pos[1] > CMD_MAX_HEIGHT) {
            return;
        }

        drawText(cmdFontId, &fontColor, pos, current->line);
        pos[1] += cmdFontHeight;

    finish:
        currentLineNumber++;
        current = current->next;
    }
}