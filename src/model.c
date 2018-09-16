#include "model.h"

void modelUpdate(Model* model, TimeStep* timeStep) {
    animationUpdate(&model->animation, timeStep);
}

void modelDraw(Model* model) {
    size_t frame = animationCurrentFrame(&model->animation);
    if(frame < model->numOfFrames) {
        drawSprite(model->sprites[frame]);
    }
}