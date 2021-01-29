#ifndef __ShipUpgrade_ShieldGenerator_H__
#define __ShipUpgrade_ShieldGenerator_H__

#include "../modules/RGNDS_Engine/transform.h"

#include "shipstats.h"
#include "shipupgrade.h"
#include "shipupgrade_shield.h"

class ShipUpgrade_ShieldGenerator : public ShipUpgrade {
public:
    ShipUpgrade_ShieldGenerator();
    virtual~ShipUpgrade_ShieldGenerator();

    bool init(ShipStats* stats, int* controls, glImage* gfx); 
    bool update(ShipStats* shipstats, Ship* ship, float deltaTime, int keys_held, int keys_up, int keys_justpressed);
    void draw(RGNDS::Transform& ship); 

    static ShipUpgrade_Shield shield;

protected:

private:
    byte* uses;
    int* controls;
    bool deployShield;

};

#endif
