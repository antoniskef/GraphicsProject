#include "particleSystem.h"

#define W_WIDTH 1024
#define W_HEIGHT 768

ParticleSystem::ParticleSystem(Start* start){
    this->start = start;
}

void ParticleSystem::createParticles(){

    Particle* p = new Particle(start);
    particles.push_back(p);
    p->create();

}

void ParticleSystem::drawParticles(mat4 transfer){

    for (int i = 0; i<particles.size(); i++){
        particles[i]->draw(transfer);
    }
}

void ParticleSystem::update(float deltaTime) {

    for (int i = 0; i<particles.size(); i++){
        particles[i]->update(deltaTime);
    }

}