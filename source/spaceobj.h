#ifndef __SPACEOBJ_H__
#define __SPACEOBJ_H__

#include <functional>

#include <nds.h>

#include "../modules/RGNDS_Engine/transform.h"
#include "wraparoundrenderer.h"

class SpaceObj : public RGNDS::Transform {
public:
    struct MainGameUpdateData {
        float deltaTime; 
        int keys_held; 
        int keys_up; 
        int keys_justpressed; 
        touchPosition touch;
    };

    struct MainGameDrawData {
        float deltaTime;
        RGNDS::Engine::Screen screen;
    };

    SpaceObj();
    SpaceObj(float objectRadius);

    RGNDS::Point<double> velocity;

    void updatePosition();
    void getCollisionSphere(RGNDS::Point<float> *pos, float *radius);

    void draw(std::function<void(RGNDS::Transform*)> drawingLambda);

    virtual void onUpdate(MainGameUpdateData* ) {};
    virtual void onDraw(MainGameDrawData* ){};
    virtual short getScoreValue() { return 0; }

    virtual void kill();
    bool isAlive();

protected:
    WrapAroundRenderer renderer;
    float objRadius;
    bool bIsAlive;


};

#endif
