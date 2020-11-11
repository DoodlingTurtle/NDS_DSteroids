#include "animation.h"

Animation::Animation(RGNDS::Transform *startObj) {
    changeAnimatedObject(startObj);
}

Animation::~Animation(){}

void Animation::play() {
    running=true;
}
void Animation::pause() {
    running = false;
}
void Animation::stop() {
    timepassed = 0;
    running = false;
}

void Animation::calculateDelta() {
    if(currentObj != nullptr) {
        targetDelta.pos.x = target->pos.x - currentObj->pos.x;
        targetDelta.pos.y = target->pos.y - currentObj->pos.y;
        targetDelta.scale = target->scale - currentObj->scale;
        targetDelta.setAngle(target->ang - currentObj->ang);
    }
}

void Animation::changeAnimatedObject(RGNDS::Transform *obj) {
    this->currentObj = obj;
    if(currentObj != nullptr) {
        currentLerp.pos.x = currentObj->pos.x;
        currentLerp.pos.y = currentObj->pos.y;
        currentLerp.scale = currentObj->scale;
        currentLerp.setAngle(currentObj->ang);

        calculateDelta();

        stop();
    }
}

void Animation::moveTo(RGNDS::Transform* tra, int time) {

    target = tra;
    
    calculateDelta();

    timespan = time;
    stop();
}

void Animation::update(float deltaTime) {
    if(!running or currentObj == nullptr) return;

    timepassed += deltaTime*1000.0f;
    if(timepassed >= timespan) {
        currentLerp.pos.x = target->pos.x;
        currentLerp.pos.y = target->pos.y;
        currentLerp.scale = target->scale;
        currentLerp.setAngle(target->ang);
        running = false;
    }
    else {
        float delta = timepassed / timespan;
        currentLerp.pos.x = currentObj->pos.x + targetDelta.pos.x * delta;
        currentLerp.pos.y = currentObj->pos.y + targetDelta.pos.y * delta;
        currentLerp.scale = currentObj->scale + targetDelta.scale * delta;
        currentLerp.setAngle(currentObj->ang + targetDelta.ang * delta); 
    }
}

void Animation::getLerp(RGNDS::Transform *tr) {
    if(tr == currentObj) return;

    tr->pos.x = currentLerp.pos.x;
    tr->pos.y = currentLerp.pos.y;
    tr->scale = currentLerp.scale;
    tr->setAngle(currentLerp.ang);

}