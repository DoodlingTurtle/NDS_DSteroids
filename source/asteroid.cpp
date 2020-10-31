#include "asteroid.h"

#include "../modules/RGNDS_Engine/engine.h"

#include "gamestatemaingame.h"
#include "ship.h"

#include <math.h>

#include "collision.h"

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

RGNDS::Broadcast Asteroid::broadcast;
std::vector<Shot*> Asteroid::shots;

Ship* Asteroid::ship = nullptr;

std::function<void(int, void*)> Asteroid::onShipAction = [](int event, void* data) {
	switch(event) {
		case bceSpawn:
			Engine_Log("Asteroid register ship spawn event");
			ship = (Ship*)data;
			break;

		case bceDead:
			Engine_Log("Asteroid register ship dead event");
			ship = nullptr;
			break;
	}
};


std::function<void(int, void*)> Asteroid::onShotAction = [](int event, void* obj){
    switch(event) {
        case bceSpawn: {

            Engine_Log("Register Shot to asteroids");
            Asteroid::shots.push_back((Shot*)obj);

        } break;

        case bceDead: {
            Engine_Log("Remove Shot from asteroids");
            for(int a = Asteroid::shots.size()-1; a >= 0; a--)
                if(Asteroid::shots.at(a) == (Shot*)obj)
                    Asteroid::shots.erase(Asteroid::shots.begin()+a);

        } break;
    }
};


Asteroid::Asteroid() : 
    RGNDS::GL2D::PolyShape(18, nullptr, GL_QUAD_STRIP) 
    , SpaceObj(16.0f)    
{
    generateShape();
    
    onGameHeartbeat = [this](int event, void* data) {
        switch(event) {
            case bceTick:
                this->update(((MainGameUpdateData*)data)->deltaTime);
                break;

            case bceDraw:

                if(
                    pos.y < 16 || pos.y > SCREEN_HEIGHT2 - 16
                    ||
                    (((MainGameDrawData*)data)->screen == 0 && pos.y < SCREEN_HEIGHT+16)
                    ||
                    (((MainGameDrawData*)data)->screen == 1 && pos.y > SCREEN_HEIGHT-16)
                ) {
                    SpaceObj::draw([this](RGNDS::Transform* tr){
                        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 14, 11, 10), tr);
                    });
                }
                break;
        }
    };


}

void Asteroid::update(float deltatime) {

    setAngleRel(PI2 * (deltatime * spinSpeed));
    updatePosition();

    // Check for collision with Ship
    if(this->ship != nullptr) {
        RGNDS::Point <float> p;
        float r;
        ship->getCollisionSphere(&p, &r);

        if(RGNDS::Collision::checkCircleOnCircle(
            &this->pos, this->scale * 14,
            &p, r
        )) broadcast.transmit(bceHitPlayer, this);
    }

    for(Shot* s : shots) {
        if(RGNDS::Collision::checkCircleOnCircle(
            &this->pos, this->scale * 14, &s->pos, 2
        )) {
            s->kill();
            broadcast.transmit(bceDead, this);
        } 

    }
}

void Asteroid::generateShape() {
    float angSteps = PI2 / ((numPoints-2) / 2);
    float radius = 16;
    float ang = 0;
    int a;

    // Setup Shape
    float pointdist[numPoints + 1];

    for(a = 0; a < (numPoints-2)/2 + 1; a++){
        pointdist[a] = radius
            + ((RandF() * 8 - 4) * (a%2 > 0));
    }

    this->setPoint(0, 0, 0);
    this->setPoint(1, radius, 0);

    for(a = 0; a < numPoints; a+=2) {
        ang += angSteps;

        this->setPoint(a, 0, 0);
        this->setPoint(
            a+1,
            cos(ang) * pointdist[a/2],
            sin(ang) * pointdist[a/2]
        );
    }

    return;
}

void Asteroid::bringBackToLife(RGNDS::Broadcast* gameHeartbeat, RGNDS::Point<float> pos, bool generateNewShape, float scale) {
    Engine_Log("Bring asteroid back to life");

    setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360� every 16 Seconds (at max spinspeed)

    this->pos.x = pos.x;
	this->pos.y = pos.y;
    this->scale = scale;

    if(generateNewShape)
        generateShape();


    gameHeartbeat->subscribe(&onGameHeartbeat);
}

void Asteroid::kill(RGNDS::Broadcast* gameHeartbeat) {
    gameHeartbeat->unsubscribe(&onGameHeartbeat);
}

Asteroid::~Asteroid() {
}
