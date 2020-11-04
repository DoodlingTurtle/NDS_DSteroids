#ifndef __PARTICLES_H__
#define __PARTICLES_H__ 1

#include "../modules/RGNDS_Engine/transform.h"
#include <vector>

namespace ParticleSystem
{
    class Particle : public RGNDS::Transform {
    public:
        Particle();
        virtual ~Particle();

        virtual void attachToVector(std::vector<RGNDS::Point<float>>) { return; };
        virtual int numberOfVertices() { return 0; };

    protected:
        float velocity;
        float lifetime;

    };


    class Emitter {
    public:
        RGNDS::Point<float> location;

        Emitter(float x, float y, int numberofparticles, Particle* prototypeParticle);
        virtual ~Emitter();

        void update(float deltaTime);
        void draw();

    protected:
        Particle* protoParticle;
        
        
    };





} // namespace ParticleSystem

#endif;