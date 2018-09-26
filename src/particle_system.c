#include "particle_system.h"


void emitterInit(Emitter* em) {
    memset(em, 0, sizeof(Emitter));
}
void emitterFree(Emitter* em) {

}

void emitterUpdate(Emitter* em, TimeStep* timeStep) {

}

void emitterDraw(Emitter* em) {

}

void emitterKill(Emitter* em) {

}

void emitterStart(Emitter* em) {

}

void emitterStop(Emitter* em) {

}

void emitterPause(Emitter* em) {

}

void emitterReset(Emitter* em) {

}

void emitterIsDone(Emitter* em) {

}

void emitterAddGenerator(Emitter* em, ParticleGenerator gen) {
    if((em->numGenerators + 1) > MAX_MODIFIERS) {
        logger(ERROR_LEVEL, "Reached max (%d) number of generators for emitter: %s", MAX_MODIFIERS, em->name);
        return;
    }

    em->generators[em->numGenerators++] = gen;
}

void emitterAddUpdater(Emitter* em, ParticleUpdate update) {
    if((em->numUpdaters + 1) > MAX_MODIFIERS) {
        logger(ERROR_LEVEL, "Reached max (%d) number of updaters for emitter: %s", MAX_MODIFIERS, em->name);
        return;
    }

    em->updaters[em->numUpdaters++] = update;
}

void emitterAddDrawer(Emitter* em, ParticleDraw drawer) {
    if((em->numDrawers + 1) > MAX_MODIFIERS) {
        logger(ERROR_LEVEL, "Reached max (%d) number of drawers for emitter: %s", MAX_MODIFIERS, em->name);
        return;
    }

    em->drawers[em->numDrawers++] = drawer;
}
