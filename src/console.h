#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h"

/**
 * Command that can be executed thru the console
 */
typedef void (*Command)(const char* format, ...);

void consoleInit();
void consoleFree();

void consolePrintf(const char* format, ...);
void consoleExecute(const char* format, ...);
void consoleAddCommand(const char* name, Command cmd);
void consoleRemoveCommand(const char* name);

void consoleToggle();
void consoleDeactivate();
int  consoleIsActive();

void consoleUpdate(TimeStep* timeStep);
void consoleDraw();
void consoleInputs();

#endif
