#include "asteroid.h"

#include "ship.h"

#include <math.h>

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
}

void Asteroid::draw(int screen) {
    if(!alive) return;

    RGNDS::Point<float> p = tra->pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        tra->pos = renderer.getInstance(a);
        RGNDS::GL2D::PolyShape::draw(Engine_Color16(1, 14, 11, 10), screen, tra);
    }

    tra->pos = p;
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
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360° every 16 Seconds (at max spinspeed)

    tra->pos = pos;
    tra->scale = scale;

    if(generateNewShape)
        generateShape();

    alive = true;
}

void Asteroid::kill() {
    alive = false;
}

void Asteroid::onBroadcast(int channel, int event, void* broadcastdata) {
    if(channel == bchShip) {
        if(event == bceMove) {
            RGNDS::Point<float> dist = ((Ship*)broadcastdata)->pos - tra->pos;
            dist *= dist;
            float fdist = sqrt(dist.x + dist.y);

            if((16 * tra->scale) + (16 * ((Ship*)broadcastdata)->scale) > fdist)
                broadcast.transmit(bceDead, this);

            Engine_Log("dist to ship: " << fdist);
        }
    }
}


Asteroid::~Asteroid()
{
    delete tra;
}
