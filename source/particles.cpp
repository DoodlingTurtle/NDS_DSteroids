#include "particles.h"

#include "../modules/RGNDS_Engine/gl2d.h"

namespace ParticleSystem {

    Particle::Particle(){};
    Particle::~Particle(){};

    int Particle::numberOfVerticesPerParticle() { return 3; }




    Emitter::Emitter(float x, float y, int num, Particle* proto) {
        transform.pos.x = x;
        transform.pos.y = y;
        transform.scale = 1;

        protoParticle = proto;

        particles.reserve(num);
        spawnNewParticles(num);
    }

    void Emitter::spawnNewParticles(int cnt) {
        Particle* p;
        for(int a = 0; a < cnt; a++) {
            p = protoParticle->getNewInstance(particles.size()-1);
            if(p!=nullptr)
                particles.push_back(p);
        }
    }

    Emitter::~Emitter() {
        for(auto p : particles)
            delete p;

        particles.clear();
    }

    bool Emitter::update(float deltaTime) {
        Particle* p;
        int a;

        vertices.clear();
        
        for(a = particles.size()-1; a >= 0; a--) {
            p = particles.at(a);    

            if(!p->update(deltaTime)) {
                delete p;
                onParticleDeath(p);
                particles.erase(particles.begin() + a);
            }
        }

        if(particles.size() == 0)
            onNoParticlesLeft();

        if(particles.size() > 0) {
            vertices.reserve(particles.size() * protoParticle->numberOfVerticesPerParticle());
        
            a = 0;
            for(auto p : particles) {
                p->attachToVector(deltaTime, a, &vertices);
                a++;
            }

            return true;
        }
        
        return false;
    }

    void Emitter::draw(int color, int alpha, float zDepth) {
        if(vertices.size() > 0)
            RGNDS::GL2D::glShape(GL_TRIANGLES, color, vertices.size(), vertices.data(), &transform, alpha, zDepth);
    }

}
