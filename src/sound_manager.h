#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <stdint.h>

#include "SDL_mixer.h"
#include "common.h"

typedef int SoundId;
typedef int SoundSource;
#define INVALID_SOUNDID 0

void soundManagerInit();
void soundManagerFree();

SoundId loadSound(const char* filename);
void freeSound(SoundId soundId);

SoundSource playSound(SoundId soundId, int loop);
void stopSound(SoundSource source);
void pauseSound(SoundSource source);
void resumeSound(SoundSource source);
void setVolumeSound(SoundSource source, int volume);

int isSoundPaused(SoundSource source);
int isSoundPlaying(SoundSource source);
int soundVolume(SoundSource source);
//void positionSound(SoundId soundId, Vec2 pos);
//void panSound(SoundId soundId, uint8_t left, uint8_t right);

#endif
