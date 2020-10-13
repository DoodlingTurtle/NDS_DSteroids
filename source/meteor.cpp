#include "meteor.h"

#include <math.h>

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

Meteor::Meteor() : RGNDS::Engine::PolyObj(10, nullptr) {

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);

    float angSteps = PI2 / 10;
    float radius = 16;
    float ang = 0;

    pos.x = SCREEN_WIDTH/2;
    pos.y = SCREEN_HEIGHT / 2;

    for(int a = 0; a < 30; a+=3) {
        points[a  ] = { 0, 0 };
        points[a+1] = { cos(ang) * radius, sin(ang) * radius };

        radius += (RandF() * 16.0f) - 8;
        points[a+2] = { cos(ang+angSteps) * radius, sin(ang+angSteps) * radius };

        ang+=angSteps;
    }



    setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;

}

void Meteor::update(float deltatime) {
    setAngleRel(PI2 * (deltatime * 0.0625));    // Spin by 360° every 16 Seconds
    pos += velocity;
    renderer.updateDrawingInstances(&pos, 24);


}

void Meteor::draw(int screen) {
    RGNDS::Point<float> p = pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++){
        pos = renderer.getInstance(a);
        RGNDS::Engine::PolyObj::draw(0xffff, screen);
    }

    pos = p;
}

Meteor::~Meteor()
{
    //dtor
}
