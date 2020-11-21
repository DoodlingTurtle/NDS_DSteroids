#include "shipupgrade_shield.h"
#include "../modules/RGNDS_Engine/engine.h"

ShipUpgrade_Shield::ShipUpgrade_Shield() 
{
    circle = RGNDS::GL2D::PolyShape::createCircle( 28, 16 );
}

ShipUpgrade_Shield::~ShipUpgrade_Shield() 
{
    delete circle;
}

void ShipUpgrade_Shield::draw(RGNDS::Transform& ship) 
{
    circle->draw(Engine_Color16(1, 8, 31, 31), &ship, 7, 3);
}

void ShipUpgrade_Shield::update(float dt) 
{

}

float ShipUpgrade_Shield::getRadius() {
    return 28.0f;
}
