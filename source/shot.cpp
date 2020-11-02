#include "./shot.h"
#include "broadcastchannels.h"

#include "gamestatemaingame.h"

Shot _instances[32];
std::vector<SpaceObj*>* Shot::shotGameObjects = nullptr;

void Shot::cleanup() {
    for(int a = 0; a < 32; a++)
        _instances[a].kill();
}

void Shot::Spawn(float ang, RGNDS::Point<float> *pos) {
    if(shotGameObjects == nullptr) return;

    for(int a = 0; a < 32; a++) {
        if(_instances[a].bIsAlive) continue;

        Shot* shot = &_instances[a];
        shot->pos.x = pos->x;
        shot->pos.y = pos->y;
        shot->setAngle(ang);
        shot->lifetime = 1000;
        shot->moveInDirection(8.0);
        shot->bIsAlive = true;
        shotGameObjects->push_back(shot);
        Engine_Log("Shot fired: " << shot);
        return;
    }

    Engine_Log("Shot-Limit of 32 exceded");
}

Shot::Shot()
    :PolyShape(3, (const RGNDS::Point<double>[3]){
        {  2,  0 },
        { -2, -2 },
        { -2,  2 }
    }, GL_TRIANGLES)
    ,SpaceObj(3.0f)    
{
    scale=1;
    bIsAlive = false;
    velocity.x = 0;
    velocity.y = 0;
}

Shot::~Shot() {}

void Shot::onDraw(SpaceObj::MainGameDrawData* d) {
    SpaceObj::draw([this](RGNDS::Transform *tr){
        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 31, 31, 31), tr, 21 * ((float)lifetime/1000.0f) + 10);
    });
}

void Shot::onUpdate(SpaceObj::MainGameUpdateData* dat) {

    float deltaTime =dat->deltaTime;

    lifetime -= deltaTime * 250;        // 1000 / 4 = 4 Seconds of lifetime
    if(lifetime <= 0) {
        this->bIsAlive = false;
    }
    else {
        moveInDirection(64.0f * deltaTime);
        updatePosition();
    }
}
