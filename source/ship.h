#ifndef SHIP_H
#define SHIP_H 1

#include <vector>
#include <nds.h>
#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "./wraparoundrenderer.h"
#include "./broadcastchannels.h"
#include "../modules/RGNDS_Engine/broadcast.h"

#include "shipengine.h"
#include "spaceobj.h"
#include "shipupgrade.h"
#include "shipupgrade_shield.h"

#include "shot.h"

#include "./particles.h"


class ShipExplosionParticle 
  : public ParticleSystem::Particle
  , public RGNDS::Transform 
{
public:
    static ShipExplosionParticle proto;

    ShipExplosionParticle();
    ~ShipExplosionParticle();

    bool update(float deltaTime);
    void attachToVector(float deltaTime, int renderIndex, std::vector<RGNDS::Point<double>>*);
    ShipExplosionParticle* getNewInstance(int index);


protected:
    float lifetime;
    float velocity;
};


class Ship : public SpaceObj {
    public:
        static RGNDS::Broadcast broadcast;

        ShipEngine ph;

        Ship();
        virtual ~Ship();

        void update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch);
        void reset();

        std::function<void(int, void*)> heartbeat;

        void onUpdate(SpaceObj::MainGameUpdateData*);
        void onDraw(SpaceObj::MainGameDrawData*);

        bool gotHit(SpaceObj* culprit);

    protected:
        ShipUpgrade_Shield* currentShield; 

    private:

        void clearUpgrades();

        float angRes = PI/20;
        bool thrusting = false;
        bool shieldIsUp = false;

        RGNDS::GL2D::PolyShape* shaBody;
        RGNDS::GL2D::PolyShape* shaThruster;

        std::vector<ShipUpgrade*> upgrades;
};


class ShipExplosion : public ParticleSystem::Emitter, public SpaceObj {
public:
    ShipExplosion(Ship* ship);
    ~ShipExplosion();

    void onUpdate(SpaceObj::MainGameUpdateData*);
    void onDraw(SpaceObj::MainGameDrawData*);

    void onNoParticlesLeft();
};


#endif // SHIP_H
