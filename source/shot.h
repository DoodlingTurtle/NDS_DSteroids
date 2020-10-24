#ifndef __SHOT_H__
#define __SHOT_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"

#include "spaceobj.h"

class Shot : public RGNDS::GL2D::PolyShape, public SpaceObj {

public:
    
    Shot( float angle, RGNDS::Point<float>* pos );
    virtual ~Shot();

    void draw();
    bool update( float deltaTime );

private:
    int lifetime = 1000;

};

#endif