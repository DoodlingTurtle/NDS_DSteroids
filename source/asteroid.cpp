#include "asteroid.h"

#include "ship.h"

#include <math.h>

#include "collision.h"

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

Broadcast Asteroid::broadcast(bchAsteroid);

Asteroid::Asteroid() : 
    RGNDS::GL2D::PolyShape(18, nullptr, GL_QUAD_STRIP) 
    , SpaceObj(16.0f)    
{
    generateShape();
    this->alive = false;
}

void Asteroid::update(float deltatime) {
    if(!alive) return;

    setAngleRel(PI2 * (deltatime * spinSpeed));
    updatePosition();

    broadcast.transmit(bceMove, this);
}

void Asteroid::draw() {
    if(!alive) return;

    SpaceObj::draw([this](RGNDS::Transform* tr){
        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 14, 11, 10), tr);
    });
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

void Asteroid::bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale) {
    setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360� every 16 Seconds (at max spinspeed)

    pos = pos;
    scale = scale;

    if(generateNewShape)
        generateShape();

    alive = true;
}

void Asteroid::kill() {
    alive = false;
}

void Asteroid::onBroadcast(int channel, int event, void* bcdata) {
// React to Ship Movement
    if(channel == bchShip && event == bceMove) {
        RGNDS::Point <float> p;
        float r;
        ((Ship*)bcdata)->getCollisionSphere(&p, &r);

        if(RGNDS::Collision::checkCircleOnCircle(
            &this->pos, this->scale * 14,
            &p, r
        )) broadcast.transmit(bceHitPlayer, this);
    }
}


Asteroid::~Asteroid() {
}
