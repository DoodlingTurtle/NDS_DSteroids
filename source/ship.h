#ifndef SHIP_H
#define SHIP_H #include "shipstats.h"

#include <vector>
#include <nds.h>
#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d.h"

#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "./wraparoundrenderer.h"
#include "./broadcastchannels.h"
#include "../modules/RGNDS_Engine/broadcast.h"

#include "shipstats.h"
#include "shipengine.h"
#include "shipupgrade_shield.h"
#include "shipupgrade_shieldgenerator.h"
#include "spaceobj.h"
#include "shipupgrade.h"

#include "shot.h"

#include "./particles.h"

#include "./res/Ship.h"

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

        void addUpgrade(ShipUpgrade* upgrade);

        std::function<void(int, void*)> heartbeat;

        void onUpdate(SpaceObj::MainGameUpdateData*);
        void onDraw(SpaceObj::MainGameDrawData*);

        bool gotHit(SpaceObj* culprit);
        bool shieldIsActive();

        int* controlls;
        ShipStats* stats;

    protected:
        ShipUpgrade_Shield* currentShield;
        ShipUpgrade_ShieldGenerator shieldgenerator;

    private:

        void clearUpgrades();

        float angRes = PI/20;
        bool thrusting = false;
        bool shieldIsUp = false;

        RGNDS::GL2D::PolyShape* shaBody;
        RGNDS::GL2D::PolyShape* shaThruster;

        glImage gfx[3];

        std::vector<ShipUpgrade*> upgrades;
        std::vector<ShipUpgrade*> newUpgrades; 
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
