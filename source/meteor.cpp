#include "meteor.h"

#include <math.h>

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)

Meteor::Meteor() : RGNDS::Engine::PolyObj(8, nullptr) {

    Engine_Log("number of Vertices: " << numPoints);

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);

    float angSteps = PI2 / (numPoints/3);
    float radius = 16;
    float ang = 0;
    int a;
    float d1, d2;

    pos.x = SCREEN_WIDTH/2;
    pos.y = SCREEN_HEIGHT / 2;

    float pointdist[numPoints / 3 + 1];

    for(a = 0; a < numPoints/3 + 1; a++){
        pointdist[a] = radius
            + ((RandF() * 8 - 4) * (a%2 > 0));
    }



    for(int a = 0; a < numPoints; a+=3) {

        d1 = pointdist[a / 3];
        d2 = pointdist[a / 3 + 1];

        points[a  ] = { 0, 0 };
        points[a+1] = { cos(ang) * d1, sin(ang) * d1 };

        radius += (RandF() * 16.0f) - 8;
        points[a+2] = { cos(ang+angSteps) * d2, sin(ang+angSteps) * d2 };

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
