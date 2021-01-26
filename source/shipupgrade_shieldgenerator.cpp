#include "shipupgrade_shieldgenerator.h"
#include "ship.h"
#include "shipupgrade_shield.h"

ShipUpgrade_Shield ShipUpgrade_ShieldGenerator::shield;

ShipUpgrade_ShieldGenerator::ShipUpgrade_ShieldGenerator() {}
ShipUpgrade_ShieldGenerator::~ShipUpgrade_ShieldGenerator() {}


void ShipUpgrade_ShieldGenerator::draw(RGNDS::Transform& ship) {

    RGNDS::Transform tra;
    tra.scale = 0.2f;
    tra.pos.x = 232;
    tra.pos.y = 32; 

    for(int a = 0; a < uses; a++) {
        shield.circle->draw(Engine_Color16(1, 8, 31, 31), &tra);

        tra.pos.y += 12;
    }

};

bool ShipUpgrade_ShieldGenerator::init(ShipStats *stats) {

    if(stats->shielduses > 0) {
        uses = stats->shielduses;
        deployShield = true;
        return true;
    }

    uses = 0;
    return false;
}

bool ShipUpgrade_ShieldGenerator::update(ShipStats *shipstats, Ship *ship, float deltaTime, int keys_held, int keys_up, int keys_justpressed) {

    deployShield |= (uses > 0 && keys_justpressed&KEY_A && !ship->shieldIsActive());
    

    if(deployShield) {
        uses--;
        ship->addUpgrade(&shield);
        deployShield = false;
    }

    return (uses > 0);
}
