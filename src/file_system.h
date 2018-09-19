#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <SDL.h>
#include <physfs.h>

void fileSystemInit(const char* arg);
void fileSystemFree();

int fileSystemFileExist(const char* filename);
SDL_RWops* fileSystemReadFully(const char* filename);

#endif
