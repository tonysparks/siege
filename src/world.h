#ifndef WORLD_H
#define WORLD_H

#include "common.h"

typedef struct World {

} World;

void worldInit(World* w);
void worldFree(World* w);

void worldUpdate(TimeStep* timeStep);
void worldDraw();

#endif
