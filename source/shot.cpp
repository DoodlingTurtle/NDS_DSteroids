#include "./shot.h"

Shot::Shot( float angle, RGNDS::Point<float>* pos )
    :PolyShape(3, (const RGNDS::Point<double>[3]){
        {  2,  0 },
        { -2, -2 },
        { -2,  2 }
    }, GL_TRIANGLES)
    ,SpaceObj(3.0f)    
{
    setAngle(angle);
    this->pos = *pos;
    Engine_Log("Pos: " << this->pos.x << this->pos.y);
    scale=1;
}

Shot::~Shot() {}

void Shot::draw() {
    SpaceObj::draw([this](RGNDS::Transform *tr){
        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 31, 31, 31), tr, 21 * ((float)lifetime/1000.0f) + 10);
    });
}

bool Shot::update(float deltaTime) {

    lifetime -= deltaTime * 250;        // 1000 / 4 = 4 Seconds of lifetime
    if(lifetime <= 0)
        return false;    

    moveInDirection(64.0f * deltaTime);
    updatePosition();

    return true;
}