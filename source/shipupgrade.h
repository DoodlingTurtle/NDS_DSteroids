#ifndef __ShipUpgrade_H__
#define __ShipUpgrade_H__

#include "../modules/RGNDS_Engine/transform.h"
#include "shipstats.h"

class Ship;

class ShipUpgrade {
public:
    virtual ~ShipUpgrade(){};

    virtual bool init(ShipStats* shipstats) = 0;
    virtual void draw(RGNDS::Transform& ship) = 0;
    virtual bool update(ShipStats* shipstats, Ship* ship, float deltaTime, int keys_held, int keys_up, int keys_justpressed) = 0;

};

#endif
