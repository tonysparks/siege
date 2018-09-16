#ifndef MODEL_H
#define MODEL_H

#include "animation.h"
#include "renderer.h"

#define MAX_MODEL_FRAMES 16

typedef struct Model {
    Animation animation;
    SpriteId  sprites[MAX_MODEL_FRAMES];
    size_t    numOfFrames;
} Model;

void modelUpdate(Model* model, TimeStep* timeStep);
void modelDraw(Model* model);

#endif
