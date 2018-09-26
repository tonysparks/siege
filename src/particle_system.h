#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "common.h"
#include "renderer.h"

#define MAX_PARTICLES 1024
#define MAX_MODIFIERS 6
#define MAX_NAME 32

typedef struct Emitter Emitter;
typedef struct Particles Particles;

////////////////////////////
// Particle modifiers
////////////////////////////

// Determines when and how particles spawn (defines their starting properties)
typedef void (*ParticleGenerator)(TimeStep* timeStep, Particles* particles);

// Determines how a particle changes throughout its lifetime
typedef void (*ParticleUpdate)(TimeStep* timeStep, Particles* particles);

// Determines how to render the particle
typedef void (*ParticleDraw)(Particles* particles);



/**
 * Structure of Arrays for particles
 */
typedef struct Particles {
    int         numAliveParticles;
    Emitter*    em;

    Vec2        pos[MAX_PARTICLES];
    Vec2        vel[MAX_PARTICLES];
    int         isAlive[MAX_PARTICLES];
    long        timeToLive[MAX_PARTICLES];
    float       scale[MAX_PARTICLES];
    float       rotation[MAX_PARTICLES];
    float       speed[MAX_PARTICLES];
    Color       color[MAX_PARTICLES];
    SpriteId    sprite[MAX_PARTICLES];
} Particles;


/**
 * The source of where particles should be emitted from (their starting point at
 * which ParticleGenerator is referenced from)
 */
typedef struct Emitter {
    ParticleGenerator generators[MAX_MODIFIERS];
    size_t            numGenerators;

    ParticleUpdate    updaters[MAX_MODIFIERS];
    size_t            numUpdaters;
    
    ParticleDraw      drawers[MAX_MODIFIERS];
    size_t            numDrawers;

    char      name[MAX_NAME];          /* for debug purposes */
    long      timeToLive;
    Vec2      pos;
    Particles particles; 
    int       dieInstantly;
    int       kill;
    int       isPersistent;
} Emitter;

void emitterInit(Emitter* em);
void emitterFree(Emitter* em);

void emitterUpdate(Emitter* em, TimeStep* timeStep);
void emitterDraw(Emitter* em);

void emitterKill(Emitter* em);
void emitterStart(Emitter* em);
void emitterStop(Emitter* em);
void emitterPause(Emitter* em);
void emitterReset(Emitter* em);
void emitterIsDone(Emitter* em);

void emitterAddGenerator(Emitter* em, ParticleGenerator gen);
void emitterAddUpdater(Emitter* em, ParticleUpdate update);
void emitterAddDrawer(Emitter* em, ParticleDraw drawer);

#endif
