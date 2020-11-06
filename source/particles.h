#ifndef __PARTICLES_H__
#define __PARTICLES_H__ 1

#include "../modules/RGNDS_Engine/transform.h"
#include <vector>

namespace ParticleSystem
{
    class Particle {
    public:
        Particle();
        virtual ~Particle();

        /** \brief create a new instance of this particle */ 
        virtual Particle* getNewInstance(int renderIndex) = 0;
        
        /** \brief called, by the emmitter on every update
         * 
         * \return false = particle has died, true particle is still alive
         */
        virtual bool update(float deltaTime) = 0;

        /** \brief gets the coordinates of this particle to render (vertices must be attached in sets of 3)
         * 
        */
        virtual void attachToVector(float deltaTime, int renderIndex, std::vector<RGNDS::Point<double>>*) = 0;

        /** \brief returns how many vertices this particular particle instance provides */
        int numberOfVerticesPerParticle();
    };


    class Emitter {
    public:
        RGNDS::Transform transform;
        
        Emitter(float x, float y, int numberofparticles, Particle* prototypeParticle);
        virtual ~Emitter();

        void spawnNewParticles(int cnt=1);

        virtual Particle* onParticleDeath(Particle* particle) { return nullptr; }       // Called, when one of the particles has reached its max lifetime
        virtual void onNoParticlesLeft() { return; }     // Called, once all particles in the list have reached its max lifetime

        /** \brief called within the update-loop
         * \param deltaTime a percentage based value, defines define how much time has passed, since the last update
         *                  example: 1.0 = 1 Second, 2.0 = 2 Seconds, 0.5 = 500 milliseconds
         * 
         * \return if there are more more particles to process, false = no more particles to process, the emitter can be deleted
         */
        bool update(float deltaTime);

        /** \brief called within the vblank interrupt
         * \param color 16bit ABGR - Color to draw the particles in
         * \param alpha 5bit alpha value (31 fully opaque, 0 = fully transparent)
         * \param zDepth layer to draw particles on
         * 
         **/ 
        void draw(int color, int alpha=31, float zDepth=0);

    protected:
        Particle* protoParticle;
        
    private:
        std::vector<Particle*> particles;
        std::vector<RGNDS::Point<double>> vertices;
        
    };

} // namespace ParticleSystem

#endif