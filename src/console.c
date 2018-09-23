#include "console.h"
#include "renderer.h"

#include "input_system.h"

#define MAX_INPUT_BUFFER 128
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

#define CMD_MAX_HEIGHT 100
#define CMD_BLINK_TIME 200

static TextBuffer cmdTextBuffer;
static char cmdInputBuffer[MAX_INPUT_BUFFER];
static int cmdInputBufferPos = 0;
static int cmdInputBufferSize = 0;
static Commands* cmdCommands = NULL;
static long cmdWaitTime = 0;
static int cmdIsActive = 0;
static ConsoleState cmdState = NONE;
static FontId cmdFontId = -1;
static int cmdFontHeight = 0;

static int cmdScrollHeight = 0;
static int cmdHeight = 0;
static int cmdBlinkTimer = CMD_BLINK_TIME;
static int cmdShowCursor = 1;

static char cmdShiftChar[255] = {
    ['1'] = '!',
    ['2'] = '@',
    ['3'] = '#',
    ['4'] = '$',
    ['5'] = '%',
    ['6'] = '^',
    ['7'] = '&',
    ['8'] = '*',
    ['9'] = '(',
    ['0'] = ')',
    ['-'] = '_',
    ['='] = '+',
    ['\\'] = '|',
    [']'] = '}',
    ['['] = '{',
    ['\''] = '"',
    [';'] = ':',
    ['/'] = '?',
    ['.'] = '>',
    [','] = '<',
    ['`'] = '~',
};

static void cmdClear(const char* args) {    
    cmdTextBuffer.numberOfLines = 0;
    cmdTextBuffer.start = cmdTextBuffer.end = &cmdTextBuffer.text[0];
    for(int i = 0; i < MAX_TEXT_LINES; ++i) {        
        memset(cmdTextBuffer.text[i].line, 0, MAX_INPUT_BUFFER);
    }

    cmdScrollHeight = 0;
}

static void scrollUp() {
    cmdScrollHeight++;

    int numLinesVisible = cmdHeight / cmdFontHeight;
    int max = MAX(cmdTextBuffer.numberOfLines - numLinesVisible, 0);
    if(cmdScrollHeight > max) {
        cmdScrollHeight = max;
    }
}

static void scrollDown() {
    cmdScrollHeight--;
    if(cmdScrollHeight < 0) {
        cmdScrollHeight = 0;
    }
}

static void consoleOnKeyEvent(KeyEvent* event) {
    if(!cmdIsActive) {
        return;
    }
    if(event->type == SDL_KEYUP) {
        return;
    }

    switch(event->keyCode) {
        case SDLK_BACKSPACE: {
            cmdInputBufferPos--;
            if(cmdInputBufferPos < 0) {
                cmdInputBufferPos = 0;
                return;
            }

            /* fall-thru */            
        }
        case SDLK_DELETE: {                    
            size_t len = cmdInputBufferSize - cmdInputBufferPos;
            if(len > 0) {
                char* dst = &cmdInputBuffer[cmdInputBufferPos];
                memmove(dst, &cmdInputBuffer[cmdInputBufferPos + 1], len);
            
                cmdInputBufferSize--;
                if(cmdInputBufferSize < 0) {
                    cmdInputBufferSize = 0;
                    cmdInputBufferPos  = 0;
                }
            }
            
            break;
        }
        case SDLK_LEFT: {
            cmdInputBufferPos--;
            if(cmdInputBufferPos < 0) {
                cmdInputBufferPos = 0;
                return;
            }

            break;
        }
        case SDLK_RIGHT: {
            cmdInputBufferPos++;
            if(cmdInputBufferPos > cmdInputBufferSize) {
                cmdInputBufferPos = cmdInputBufferSize;
                return;
            }
            
            break;
        }
        case SDLK_PAGEUP: {
            scrollUp();
            break;
        }
        case SDLK_PAGEDOWN: {
            scrollDown();
            break;
        }
        case SDLK_HOME: {
            cmdInputBufferPos = 0;
            break;
        }
        case SDLK_END: {
            cmdInputBufferPos = cmdInputBufferSize;
            break;
        }
        case SDLK_RETURN: {
            consoleExecute("%s", cmdInputBuffer);
            memset(cmdInputBuffer, 0, MAX_INPUT_BUFFER);
            cmdInputBufferPos = cmdInputBufferSize = 0;

            break;
        }
        default: {    
            if(event->keyCode > 31 && event->keyCode < 127) {
                if(cmdInputBufferPos + 1 < MAX_INPUT_BUFFER) {
                    size_t len = cmdInputBufferSize - cmdInputBufferPos;
                    if(len > 0) {
                        for(int i = 0; i < len; ++i)  {
                            cmdInputBuffer[(cmdInputBufferSize) - i] = cmdInputBuffer[(cmdInputBufferSize-1) - i];
                        }                            
                    }

                    char c = (char)event->keyCode;
                    if(event->keymod & (KMOD_SHIFT|KMOD_CAPS)) {
                        if(isalpha(c)) {
                            c -= 32;
                        }
                        else if(event->keymod & KMOD_SHIFT) {
                            char alt = cmdShiftChar[c];
                            if(alt) {
                                c = alt;
                            }
                        }
                    }

                    cmdInputBuffer[cmdInputBufferPos] = c;

                    cmdInputBufferSize++;
                    cmdInputBufferPos++;
                }
            }
        }
    }    
}

static void consoleOnMouseEvent(MouseEvent* event) {
    if(event->type == SDL_MOUSEWHEEL) {
        if(event->wheelDirection > 0) {
            scrollUp();
            
        }
        else if(event->wheelDirection < 0) {
            scrollDown();
        }
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

    consoleAddCommand("clear", &cmdClear);

    // temp
    cmdHeight = CMD_MAX_HEIGHT;
    cmdScrollHeight = 0;
    cmdIsActive = 1;
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

Commands* consoleFindCommand(const char* name, size_t len) {
    Commands* c = cmdCommands;
    while(c) {
        if(!strncmp(c->name, name, len)) {
            return c;
        }
        
        c = c->next;
    }

    return NULL;
}

void consoleExecute(const char* format, ...) {
    static char text[MAX_TEXT_LENGTH];

    va_list args;
    va_start(args, format);
    int len = vsnprintf_s(text, MAX_TEXT_LENGTH, MAX_INPUT_BUFFER, format, args);
    va_end(args);   

    consolePrintf("%s\n", text);

    char* cmdName = text;
    size_t pos = 0;
    while(pos < len) {
        char c = text[pos++];
        if(c == ' ') {
            break;
        }
        if(c == '\0') {
            break;
        }
    }

    Commands* cmd = consoleFindCommand(text, pos);
    if(cmd) {
        cmd->cmd(&text[pos]);
    }
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
    newCmd->next = NULL;
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
    cmdBlinkTimer -= timeStep->frameTime;
    if(cmdBlinkTimer < 0) {
        cmdBlinkTimer = CMD_BLINK_TIME;
        cmdShowCursor = !cmdShowCursor;
    }
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

    if(cmdInputBufferSize > 0) {
        VectorSet(a, 3, input.y + 5);
        drawText(cmdFontId, &fontColor, a, "%s", cmdInputBuffer);
    }

    if(cmdShowCursor) {
        int textWidth = 0;
        int textHeight = 0;
        fontWidthHeightLen(cmdFontId, &textWidth, &textHeight, cmdInputBufferPos, "%s", cmdInputBuffer);

        VectorSet(a, 3 + textWidth, input.y + 5);
        drawText(cmdFontId, &fontColor, a, "_");
    }

    /* Draw scrolling text */

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

        drawText(cmdFontId, &fontColor, pos, "%s", current->line);
        pos[1] += cmdFontHeight;

    finish:
        currentLineNumber++;
        current = current->next;
    }
}
