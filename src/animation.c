#include "animation.h"

size_t animationUpdate(Animation* anim, TimeStep* timeStep) {
    if(anim->isPaused) {
        return anim->currentFrame;
    }

    anim->elapsedTime += timeStep->frameTime;

    if(anim->elapsedTime > anim->frameTime) {
        anim->elapsedTime -= anim->frameTime;

        size_t desiredFrame = anim->currentFrame + 1;

        if(!anim->loop && desiredFrame >= anim->numberOfFrames) {
            desiredFrame = anim->numberOfFrames - 1;
        }

        animationSetCurrentFrame(anim, desiredFrame);
    }

    return anim->currentFrame;
}

void   animationPlay(Animation* anim) {
    anim->isPaused = 0;
}

void   animationStop(Animation* anim) {
    anim->isPaused = 1;
    anim->elapsedTime = 0;
    anim->currentFrame = 0;
}

void   animationPause(Animation* anim) {
    anim->isPaused = 1;
}

void   animationResume(Animation* anim) {
    anim->isPaused = 0;
}

void   animationReset(Animation* anim) {
    anim->elapsedTime = 0;
    anim->currentFrame = 0;
}
int    animationIsPaused(Animation* anim) {
    return anim->isPaused;
}

int    animationIsDone(Animation* anim) {
    return !anim->loop && anim->currentFrame >= (anim->numberOfFrames - 1);
}

size_t animationCurrentFrame(Animation* anim) {
    return anim->currentFrame;
}

size_t animationNumberOfFrames(Animation* anim) {
    return anim->numberOfFrames;
}

void   animationSetCurrentFrame(Animation* anim, size_t frameNumber) {
    anim->currentFrame = frameNumber % anim->numberOfFrames;
}
