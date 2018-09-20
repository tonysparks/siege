#include "console.h"
#include "renderer.h"

#define MAX_INPUT_BUFFER 1024
#define MAX_TEXT_BUFFER (1024*256)
#define MAX_COMMAND_NAME 128

typedef struct Commands {
    Command  cmd;
    char     name[MAX_COMMAND_NAME];
    struct Commands* next;
} Commands;

typedef struct TextBuffer {
    char*  start;
    char*  end;
    char   buffer[MAX_TEXT_BUFFER];
    size_t numberOfLines;
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

void consoleInit() {    
    cmdTextBuffer.numberOfLines = 0;
    memset(cmdTextBuffer.buffer, 0, MAX_TEXT_BUFFER);
    cmdTextBuffer.start = cmdTextBuffer.end = cmdTextBuffer.buffer;

    memset(cmdInputBuffer, 0, MAX_INPUT_BUFFER);
    cmdCommands = NULL;

    cmdFontId = loadFont("gfx/fonts/courier_new.ttf", 11);
    if(cmdFontId < 0) {
        logger(FATAL_LEVEL, "Unable to construct console without font!\n");
    }
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
    va_list args;
    va_start(args, format);
    int used = cmdTextBuffer.end - cmdTextBuffer.start;
    int remaining = MIN(MAX_TEXT_BUFFER - used, MAX_INPUT_BUFFER);
    int numberOfChars = vsprintf_s(cmdTextBuffer.end, remaining, format, args);    
    va_end(args);

    for(int i = 0; i < numberOfChars; ++i) {
        char c = *(cmdTextBuffer.end++);
        if(c == '\n') {
            cmdTextBuffer.numberOfLines++;
        }
    }
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

    Color fontColor = {
        .r = 1,
        .g = 0,
        .b = 0,
        .a = 1
    };
    Vec2 pos;
    VectorSet(pos, 5, 10);

    char* start = cmdTextBuffer.start;
    char* end   = cmdTextBuffer.end;

    while(start != end) {
        size_t i = 0;
        while(start != end) {
            char c = *start++;
            //if(c == )
            if(*start == '\n') {                
                break;
            }

            line[i++] = c;
        }

        drawText(cmdFontId, &fontColor, pos, line);
        pos[1] += 12;
    }
}