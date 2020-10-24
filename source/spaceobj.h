#ifndef __SPACEOBJ_H__
#define __SPACEOBJ_H__

#include <functional>

#include <nds.h>

#include "../modules/RGNDS_Engine/transform.h"
#include "wraparoundrenderer.h"

class SpaceObj : public RGNDS::Transform {
public:
    SpaceObj();
    SpaceObj(float objectRadius);

    RGNDS::Point<double> velocity;

    void updatePosition();
    void getCollisionSphere(RGNDS::Point<float> *pos, float *radius);

    void draw(std::function<void(RGNDS::Transform*)> drawingLambda);

protected:
    WrapAroundRenderer renderer;
    float objRadius;


};

#endif