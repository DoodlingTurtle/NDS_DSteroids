#include "asteroid.h"

#include "ship.h"

#include <math.h>

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

Broadcast Asteroid::broadcast(bchAsteroid);

Asteroid::Asteroid() : RGNDS::Engine::PolyObj(18, nullptr, Engine_Color16(1, 14, 11, 10), GL_QUAD_STRIP) {
    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);
    generateShape();
    this->alive = false;
}

void Asteroid::update(float deltatime) {
    if(!alive) return;

    setAngleRel(PI2 * (deltatime * spinSpeed));
    pos += velocity;
    renderer.updateDrawingInstances(&pos, 24);
}

void Asteroid::draw(int screen) {
    if(!alive) return;

    Engine_Log("Drawing Asteroid: " << this);

    RGNDS::Point<float> p = pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++){
        pos = renderer.getInstance(a);
        RGNDS::Engine::PolyObj::draw(screen);
    }

    pos = p;
}

void Asteroid::generateShape() {
    float angSteps = PI2 / ((numPoints-2) / 2);
    float radius = 16;
    float ang = 0;
    int a;
    float d1, d2;

    // Setup Shape
    float pointdist[numPoints + 1];

    for(a = 0; a < (numPoints-2)/2 + 1; a++){
        pointdist[a] = radius
            + ((RandF() * 8 - 4) * (a%2 > 0));
    }

    points[0].x = 2;
    points[0].y = 0;
    points[1].x = radius;
    points[1].y = 0;

    for(a = 0; a < numPoints; a+=2) {
        ang += angSteps;

        points[a+1].x = cos(ang) * pointdist[a/2];
        points[a+1].y = sin(ang) * pointdist[a/2];

        points[a].x = 0;
        points[a].y = 0;

    }

    return;


    for(int a = 0; a < numPoints; a+=3) {

        d1 = pointdist[a / 3];
        d2 = pointdist[a / 3 + 1];

        points[a  ] = { 0, 0 };
        points[a+1] = { cos(ang) * d1, sin(ang) * d1 };

        radius += (RandF() * 16.0f) - 8;
        points[a+2] = { cos(ang+angSteps) * d2, sin(ang+angSteps) * d2 };

        ang+=angSteps;
    }
}

void Asteroid::bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale) {
    setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360° every 16 Seconds (at max spinspeed)

    this->pos = pos;
    this->scale = scale;

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
            RGNDS::Point<float> dist = ((Ship*)broadcastdata)->pos - this->pos;
            dist *= dist;
            float fdist = sqrt(dist.x + dist.y);

            if((16 * scale) + (16 * ((Ship*)broadcastdata)->scale) > fdist)
                broadcast.transmit(bceDead, this);

            Engine_Log("dist to ship: " << fdist);
        }
    }
}


Asteroid::~Asteroid()
{
    //dtor
}
