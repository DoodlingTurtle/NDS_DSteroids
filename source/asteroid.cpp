#include "asteroid.h"

#include "ship.h"

#include <math.h>

#include "collision.h"

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

Broadcast Asteroid::broadcast(bchAsteroid);

Asteroid::Asteroid() : RGNDS::GL2D::PolyShape(18, nullptr, GL_QUAD_STRIP) {
    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);

    tra = new RGNDS::Transform();

    generateShape();
    this->alive = false;
}

void Asteroid::update(float deltatime) {
    if(!alive) return;

    tra->setAngleRel(PI2 * (deltatime * spinSpeed));
    tra->pos += velocity;
    renderer.updateDrawingInstances(&tra->pos, 24);

    broadcast.transmit(bceMove, this);
}

void Asteroid::draw() {
    if(!alive) return;

    RGNDS::Point<float> p = tra->pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        tra->pos = renderer.getInstance(a);
        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 14, 11, 10), tra);
    }

    tra->pos = p;

#ifdef TARGET_DEBUG
    RGNDS::GL2D::PolyShape *c = RGNDS::GL2D::PolyShape::createCircle(14, 16, std::max(0.001f, 1/tra->scale));
    c->draw(Engine_Color16(1, 12, 21, 31), tra);
    delete c;
#endif

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
    tra->setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360� every 16 Seconds (at max spinspeed)

    tra->pos = pos;
    tra->scale = scale;

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
            &tra->pos, tra->scale * 14,
            &p, r
        )) broadcast.transmit(bceHitPlayer, this);
    }
}


Asteroid::~Asteroid() {
    delete tra;
}
