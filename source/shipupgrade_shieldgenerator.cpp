#include "shipupgrade_shieldgenerator.h"
#include "ship.h"
#include "shipupgrade_shield.h"

#include "config.h"

ShipUpgrade_Shield ShipUpgrade_ShieldGenerator::shield;

ShipUpgrade_ShieldGenerator::ShipUpgrade_ShieldGenerator() {}
ShipUpgrade_ShieldGenerator::~ShipUpgrade_ShieldGenerator() {}


void ShipUpgrade_ShieldGenerator::draw(RGNDS::Transform& ship) {

    RGNDS::Transform tra;
    tra.scale = 0.25f;
    tra.pos.x = 229;
    tra.pos.y = 32; 

    for(byte a = 0; a < *uses; a++) {
        RGNDS::GL2D::glSprite(0, shield.gfx, &tra, 3);
        tra.pos.y += 12;
    }

};

bool ShipUpgrade_ShieldGenerator::init(ShipStats *stats, int* controls, glImage* gfx) {

    this->controls = controls;

    if(stats->shielduses > 0) {
        uses = &stats->shielduses;
        deployShield = true;
        return true;
    }

    uses = 0;
    return false;
}

bool ShipUpgrade_ShieldGenerator::update(ShipStats *shipstats, Ship *ship, float deltaTime, int keys_held, int keys_up, int keys_justpressed) {

    deployShield |= (uses > 0 && keys_justpressed&GameKeyMap[controls[GAMEINPUT_SHIELD]] && !ship->shieldIsActive());
    

    if(deployShield) {
        (*uses)--;
        ship->addUpgrade(&shield);
        deployShield = false;
    }

    return (*uses > 0);
}
