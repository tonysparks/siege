#ifndef WORLD_H
#define WORLD_H

#include "common.h"

typedef struct Game Game;

typedef struct World {
    Game* game;
} World;

void worldInit(World* w, Game* game);
void worldFree(World* w);

void worldUpdate(TimeStep* timeStep);
void worldDraw();

#endif
