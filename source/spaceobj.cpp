#include "./spaceobj.h"
SpaceObj::SpaceObj() : SpaceObj::SpaceObj(1.0f){}
SpaceObj::SpaceObj( float radius ) {
    objRadius = radius;
    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT*2, 0);
}

void SpaceObj::updatePosition() {
    pos += velocity;
    renderer.updateDrawingInstances(&(this->pos), objRadius * scale);
}

void SpaceObj::getCollisionSphere(RGNDS::Point<float> *pos, float *radius) {
    if(pos != nullptr)
        *pos = this->pos;
    if(radius != nullptr)
        *radius = scale * objRadius;
}

void SpaceObj::draw(std::function<void(RGNDS::Transform*)> drawingLambda) {

    RGNDS::Point<float> posOrig = this->pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        this->pos = renderer.getInstance(a);
        drawingLambda(this);
    }

    this->pos = posOrig;

}