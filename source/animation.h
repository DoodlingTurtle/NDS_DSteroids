#ifndef __ANIMATION_H__
#define __ANIMATION_H__ 1

#include "../modules/RGNDS_Engine/transform.h"

class Animation {
public:
    Animation(RGNDS::Transform *startObj=nullptr);
    virtual ~Animation();

    void moveTo(RGNDS::Transform* target, int overTime);
    void reset();

    void update(float deltaTime);
    void changeAnimatedObject(RGNDS::Transform *obj);

    void play();
    void pause();
    void stop();

    void getLerp(RGNDS::Transform* tr);


protected:
    RGNDS::Transform* target;
    RGNDS::Transform targetDelta;
    RGNDS::Transform* currentObj;
    RGNDS::Transform currentLerp;
    double timepassed = 0;
    double timespan = 0;

    bool running = true;

private:
    void calculateDelta();
};

#endif