#ifndef __ShipUpgrade_Shield_H__
#define __ShipUpgrade_Shield_H__

#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "shipupgrade.h"

class Ship;

class ShipUpgrade_Shield: public ShipUpgrade 
{
public:
    ShipUpgrade_Shield();
    ~ShipUpgrade_Shield();
    void draw(RGNDS::Transform& ship);
    bool update(float deltaTime);

    virtual float getRadius();

    void gotHit();

private:
    RGNDS::GL2D::PolyShape* circle;

    float lifetime;

    float lastHitTime;

};

#endif
