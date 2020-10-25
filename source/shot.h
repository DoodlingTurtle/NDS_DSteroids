#ifndef __SHOT_H__
#define __SHOT_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"

#include "../modules/RGNDS_Engine/broadcast.h"

#include "spaceobj.h"

class Shot : public RGNDS::GL2D::PolyShape, public SpaceObj {

public:
    static void Spawn(float angle, RGNDS::Point<float>* pos);
    
    static std::function<void(int, void*)> heartbeat;
    
    virtual ~Shot();
    static RGNDS::Broadcast broadcast;

    void kill();

private:
    static std::vector<Shot*> _instances;

    Shot( float angle, RGNDS::Point<float>* pos );
    
    int lifetime = 1000;

    void draw();
    bool update( float deltaTime );

};

#endif