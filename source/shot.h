#ifndef __SHOT_H__
#define __SHOT_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"

#include "../modules/RGNDS_Engine/broadcast.h"

#include "spaceobj.h"

#define MAX_SHOT_CNT 32

class Shot : public RGNDS::GL2D::PolyShape, public SpaceObj {

public:
    static std::vector<SpaceObj*>* shotGameObjects; 

    static std::vector<Shot*> getShots();
    static void Spawn(float angle, RGNDS::Point<float>* pos);
    static void cleanup();
    
    Shot();
    virtual ~Shot();

    void onDraw(SpaceObj::MainGameDrawData*);
    void onUpdate(SpaceObj::MainGameUpdateData*);

private:
    int lifetime = 1000;

};

#endif