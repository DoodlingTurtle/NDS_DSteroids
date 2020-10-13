#include "meteor.h"

#include <math.h>

Meteor::Meteor() : RGNDS::Engine::PolyObj(10, nullptr) {

    float angSteps = PI2 / 10;
    float radius = 16;
    float ang = 0;

    pos.x = SCREEN_WIDTH/2;
    pos.y = SCREEN_HEIGHT / 2;

    for(int a = 0; a < 30; a+=3) {
        points[a  ] = { 0, 0 };
        points[a+1] = { cos(ang) * radius, sin(ang) * radius };

        radius += (((float)rand() / (float)RAND_MAX) * 8.0f) - 4;
        points[a+2] = { cos(ang+angSteps) * radius, sin(ang+angSteps) * radius };

        ang+=angSteps;
    }

}

Meteor::~Meteor()
{
    //dtor
}
