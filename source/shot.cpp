#include "./shot.h"
#include "broadcastchannels.h"

#include "gamestatemaingame.h"


RGNDS::Broadcast Shot::broadcast;
std::vector<Shot*> Shot::_instances;

void Shot::Spawn(float ang, RGNDS::Point<float> *pos) {
    Shot *shot = new Shot(ang, pos);
    _instances.push_back(shot);
    broadcast.transmit(bceSpawn, shot);
}

std::function<void(int, void*)> Shot::heartbeat = [](int event, void* data){

    switch(event) {
        case bceTick: {
            float deltaTime = ((MainGameUpdateData*)data)->deltaTime;
            Shot* s;
            for(int a = Shot::_instances.size()-1; a >= 0; a--) {
                s = Shot::_instances.at(a);
                if(!(s->update(deltaTime))) {
                    broadcast.transmit(bceDead, s);
                    _instances.erase(_instances.begin()+a);
                }
            };
        } break;

        case bceDraw: {
            for(int a = Shot::_instances.size()-1; a >= 0; a--)
                _instances.at(a)->draw();
        } break;
    }
};



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
    Engine_Log("Spawn Shot Pos: " << this->pos.x << this->pos.y);
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
    if(lifetime <= 0) {
        return false;
    }
    moveInDirection(64.0f * deltaTime);
    updatePosition();

    return true;
}