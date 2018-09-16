#ifndef ANIMATION_H
#define ANIMATION_H

#include "common.h"

typedef struct Animation {
    size_t numberOfFrames;
    long   elapsedTime;
    long   frameTime;
    size_t currentFrame;
    int    loop;
    int    isPaused;
} Animation;

size_t animationUpdate(Animation* anim, TimeStep* timeStep);
void   animationPlay(Animation* anim);
void   animationStop(Animation* anim);
void   animationPause(Animation* anim);
void   animationResume(Animation* anim);
void   animationReset(Animation* anim);
int    animationIsPaused(Animation* anim);
int    animationIsDone(Animation* anim);

size_t animationCurrentFrame(Animation* anim);
size_t animationNumberOfFrames(Animation* anim);
void   animationSetCurrentFrame(Animation* anim, size_t frameNumber);


#endif
