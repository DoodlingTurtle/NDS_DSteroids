#include "shipupgrade_shield.h"
#include "../modules/RGNDS_Engine/engine.h"

ShipUpgrade_Shield::ShipUpgrade_Shield() 
{
    circle = RGNDS::GL2D::PolyShape::createCircle( 28, 16 );
    lifetime = 5000.0f;
    lastHitTime = 0;
}

ShipUpgrade_Shield::~ShipUpgrade_Shield() 
{
    delete circle;
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

    circle->draw(Engine_Color16(1, r, g, b), &ship, 3 + (4 * (lifetime / 5000.0f)), 3);
}

void ShipUpgrade_Shield::gotHit() {
    this->lastHitTime = 1000.0f;
}

bool ShipUpgrade_Shield::update(float dt) 
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
