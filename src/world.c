#include "world.h"

#include "texture_manager.h"
#include "sound_manager.h"

#include "model.h"


static TextureId hero = 0;
static FontId fontId  = -1;

static Model fireModel = {
    .animation = {
        .numberOfFrames = 4,
        .elapsedTime = 0,
        .frameTime = 1140,
        .currentFrame = 0,
        .loop = 1,
        .isPaused = 0
    },    
    .numOfFrames = 4,
};

void worldInit(World* w, Game* game) {
    w->game = game;

    hero = textureManagerLoadTexture(game->textureManager, "gfx/german_south_fire.png");

    float x = 16;
    float y = 8;

    for(size_t i = 0; i < fireModel.numOfFrames; ++i) {
        SpriteId frame = allocSprite(hero);
        Sprite* sprite = getSprite(frame);
        if(sprite) {
            fireModel.sprites[i] = frame;

            sprite->bounds.x = x;
            sprite->bounds.y = y;
            sprite->bounds.w = 35;
            sprite->bounds.h = 55;

            VectorSet(sprite->pos, 50, 60);        
            VectorSet(sprite->rotationPos, 0, 0);            
            x += 64;            
        }
    }
    
    fontId = loadFont("gfx/fonts/courier_new.ttf", 11);
    //SoundId zingId = loadSound("sfx/bullet_zing01.wav");
    //SoundSource source = playSound(zingId, -1);
}


void worldFree(World* w) {
    w->game = NULL;
}

void worldUpdate(TimeStep* timeStep) {

    modelUpdate(&fireModel, timeStep);
}

void worldDraw() {
    Vec2 pos = {20, 120};
    drawTexture(hero, pos);

  //  drawSprite(guy);

    Color color = {
        .r = 255,
        .g = 0,
        .b = 0,
        .a = 255
    };
    VectorSet(pos, 0, 0);
    //drawText(fontId, &color, pos, "Hello '%s'", "there");
    drawText(fontId, &color, pos, "Hello %d %d", fireModel.animation.currentFrame, fireModel.animation.elapsedTime);

    modelDraw(&fireModel);
}