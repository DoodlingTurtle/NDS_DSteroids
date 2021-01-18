#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "spaceobj.h"
#include "ship.h"
#include "shot.h"
#include "./particles.h"

#include "res/sb1.h"

class AsteroidParticle : public ParticleSystem::Particle, public RGNDS::Transform {
public:
    static AsteroidParticle proto;

    AsteroidParticle();
    ~AsteroidParticle();

    bool update(float deltaTime);
    void attachToVector(float deltaTime, int renderIndex, std::vector<RGNDS::Point<double>>*);
    AsteroidParticle* getNewInstance(int index);

    static float scale;

protected:
    float lifetime;

};

class AsteroidExplosion 
    : public ParticleSystem::Emitter
    , public SpaceObj 
{
public:
    AsteroidExplosion(float x=0, float y=0);

    void onUpdate(SpaceObj::MainGameUpdateData*);
    void onDraw(SpaceObj::MainGameDrawData*);

    void revive(float x, float y, float scale=1.0f);

    void onNoParticlesLeft();

};




class Asteroid 
    : public RGNDS::GL2D::PolyShape
    , public SpaceObj
{
    public:
        static Ship* ship;

        static void init(Ship* ship);
        static void deinit();
        
        Asteroid();
        virtual ~Asteroid();

        void update(float deltaTime);
       
       static std::function<void(int, void*)> onShipAction;

        void bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale);

        void onDraw(SpaceObj::MainGameDrawData*);
        void onUpdate(SpaceObj::MainGameUpdateData*);

        short getScoreValue();

    protected:
        // Objects for Asteroid to keep track of
        static std::vector<Shot*> shots;

    private:
        void generateShape();
        float spinSpeed;

        static int sounds[4];
};

#endif
