#ifndef __ShipUpgrade_H__
#define __ShipUpgrade_H__

#include "../modules/RGNDS_Engine/transform.h"

class ShipUpgrade {
public:
    virtual ~ShipUpgrade(){};

    virtual void draw(RGNDS::Transform& ship) = 0;
    virtual bool update(float deltaTime) = 0;

};

#endif
