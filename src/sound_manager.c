#include "sound_manager.h"
#include <string.h>

#define MAX_CHANNELS 128

typedef struct Sound {
    char       filename[MAX_PATH];
    Mix_Chunk* sample;
} Sound;

typedef struct SoundManager {
    Sound** sounds;
} SoundManager;

static SoundManager soundManager = {
    .sounds = NULL
};

void soundManagerInit() {
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        logger(FATAL_LEVEL, "Unable open audio device: %s \n", Mix_GetError());
        return;
    }

    Mix_Init(0);

    Mix_AllocateChannels(MAX_CHANNELS);
}
void soundManagerFree() {
    for(int i = 0; stb_sb_count(soundManager.sounds); i++) {
        Sound* sound = soundManager.sounds[i];
        if(sound) {
            freeSound(i+1);
            siegeFree(sound);
        }

        stb_sb_free(soundManager.sounds);
        soundManager.sounds = NULL;
    }

    Mix_CloseAudio();
    Mix_Quit();
}

#define IS_VALID_SOUNDID(soundId) ((soundId) > 0 && (soundId) <= stb_sb_count(soundManager.sounds))

SoundId loadSound(const char* filename) {
    for(int i = 0; stb_sb_count(soundManager.sounds); i++) {
        Sound* sound = soundManager.sounds[i];
        if(!strcmp(sound->filename, filename)) {
            return i + 1;
        }        
    }
    

    Sound* sound = NULL;
    SoundId soundId = INVALID_SOUNDID;

    for(int i = 0; i < stb_sb_count(soundManager.sounds); ++i) {
        Sound* snd = soundManager.sounds[i];
        if(snd) {
            if(!snd->sample) {
                sound = snd;
                soundId = i + 1;
                break;
            }
        }
    }

    if(!sound) {
        sound = (Sound*) siegeMalloc(sizeof(Sound));
        memset(sound->filename, 0, MAX_PATH);
        sound->sample = NULL;
        stb_sb_push(soundManager.sounds, sound);
        soundId = stb_sb_count(soundManager.sounds);
    }


    Mix_Chunk* sample = Mix_LoadWAV(filename);
    if(!sample) {
        logger(ERROR_LEVEL, "Unable to load sound '%s' : %s\n", filename, Mix_GetError());
        return INVALID_SOUNDID;
    }
    
    strcpy(sound->filename, filename);
    sound->sample = sample;

    return soundId; // invalid texture ID

}

static Sound* getSound(SoundId soundId) {
    if(!IS_VALID_SOUNDID(soundId)) {
        return NULL;
    }

    Sound* sound = soundManager.sounds[soundId - 1];
    return sound;
}

void freeSound(SoundId soundId) {
    Sound* sound = getSound(soundId);
    if(sound) {
        Mix_FreeChunk(sound->sample);
        sound->sample = NULL;
        memset(sound->filename, 0, MAX_PATH);
    }
}

SoundSource playSound(SoundId soundId, int loop) {
    Sound* sound = getSound(soundId);
    if(sound) {
        int channel = Mix_PlayChannel(-1, sound->sample, loop);
        if(channel < 0) {
            logger(ERROR_LEVEL, "Unable to play soundId '%s' : %s\n", sound->filename, Mix_GetError());            
        }

        return channel;
    }

    return -1;
}
void stopSound(SoundSource source) {
    Mix_HaltChannel(source);
}

void pauseSound(SoundSource source) {
    Mix_Pause(source);
}
void resumeSound(SoundSource source) {
    Mix_Resume(source);
}

void setVolumeSound(SoundSource source, int volume) {
    Mix_Volume(source, volume);
}

int isSoundPaused(SoundSource source) {
    return Mix_Paused(source);
}

int isSoundPlaying(SoundSource source) {    
    return Mix_Playing(source);
}

int soundVolume(SoundSource source) {
    return Mix_Volume(source, -1);
}