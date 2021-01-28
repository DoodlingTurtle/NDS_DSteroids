#include "shipupgrade_shield.h"
#include "../modules/RGNDS_Engine/engine.h"
#include "shipstats.h"

ShipUpgrade_Shield::ShipUpgrade_Shield() { }

ShipUpgrade_Shield::~ShipUpgrade_Shield() { }

bool ShipUpgrade_Shield::init(ShipStats* shipstats, int* c, glImage* gfx) { 
    
    lifetime = 5000.0f;
    lastHitTime = 0;
    this->gfx = &gfx[2];

    return true; 
}

void ShipUpgrade_Shield::draw(RGNDS::Transform& ship) 
{
    byte r=8, g=31, b=31;

    if(lastHitTime > 0) {
        float delta = lastHitTime / 1000.0f;
        r += delta * 23;
        g -= delta * 15;
        b = g;
    }

    RGNDS::Transform tra;
    tra.pos = ship.pos - 16;
    tra.pos.y -= 1;
    tra.scale = 1.01;

    glColor(Engine_Color16(1, r, g, b));
    RGNDS::GL2D::glSprite(0, gfx, &tra, 4, 3 + (4 * (lifetime / 5000.0f)));
    glColor(0xffff);
}

void ShipUpgrade_Shield::gotHit() {
    this->lastHitTime = 1000.0f;
}

bool ShipUpgrade_Shield::update(ShipStats* shipstats, Ship* ship, float dt, int keys_held, int keys_up, int keys_justpressed) 
{
    lifetime -= dt * 1000.0f;
    if(lastHitTime > 0.0f) {
        lastHitTime -= dt * 1000.0f;
        if(lastHitTime < 0)
            lastHitTime = 0;
    }
    return (lifetime > 0.0f);
}

float ShipUpgrade_Shield::getRadius() {
    return 28.0f;
}
