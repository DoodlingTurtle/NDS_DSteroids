#include "ship.h"
#include <math.h>

#define SCREEN_HEIGHT2 384

Ship::Ship() {

    shaBody = new RGNDS::GL2D::PolyShape(
          4,
          (const PointF[4]){
                { -16.0f, -16.0f }
              , { - 8.0f,   0.0f }
              , {  16.0f,   0.0f }
              , { -16.0f,  16.0f }
          },
          GL_TRIANGLE_STRIP
    );


    shaThruster = new RGNDS::GL2D::PolyShape(
         3,
         (const PointF[3]){
              { -16,   0 },
              { - 8,  -8 },
              { - 8,   8 },
         },
         GL_TRIANGLE
    );

    
// start broadcasting
    broadcast = new Broadcast(bchShip);

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    reset();

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);
}

Ship::~Ship() {
    delete broadcast;
    delete shaBody;
    delete shaThruster;
}

void Ship::update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch) {

// Process user Input
    if(keys_held&(KEY_RIGHT|KEY_A))
        this->setAngleRel(angRes);

    if(keys_held&(KEY_LEFT|KEY_Y))
        this->setAngleRel(-angRes);

    if(keys_held&KEY_START)
        this->scale += 0.05;

    if(keys_held&KEY_SELECT)
        this->scale -= 0.05;

    if(keys_held&(KEY_UP|KEY_X)) {
        thrusting = true;
        ph.accelerate(deltaTime * 1.5f);
    }
    else {
        thrusting = false;
        ph.decerlerate(1);
    }

// Update Position based on physics
    velocity += dir * ph.acceleration;

    pos += velocity;

    //TODO: (RGO) Make sure ship radius does not eclipse SCREEN_HEIGHT (192px)
    float shipRadius;
    getCollisionSphere(nullptr, &shipRadius);

    // Update Position based on Screen-Borders
    renderer.updateDrawingInstances(&(this->pos), shipRadius);

    broadcast->transmit(bceMove, this);
}

void Ship::draw() {
    RGNDS::Point<float> posOrig = this->pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        this->pos = renderer.getInstance(a);
        if(thrusting)
            shaThruster->draw(Engine_Color16(a, 31, 31,  0), this);

        shaBody->draw(Engine_Color16(a, 31,  0,  0), this);
    }

    this->pos = posOrig;

#ifdef TARGET_DEBUG
    RGNDS::GL2D::PolyShape *c = RGNDS::GL2D::PolyShape::createCircle(12, 16, std::max(0.0001f, 1/scale));
    c->draw(Engine_Color16(1, 12, 21, 31), this);
    delete c;
#endif

}

void Ship::getCollisionSphere(RGNDS::Point<float> *pos, float *radius) {
    if(pos != nullptr)
        *pos = this->pos;
    if(radius != nullptr)
        *radius = this->scale * 16.0f;
}

void Ship::reset() {
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    scale = .75;
    ang  = -PI/2;
    velocity.x = 0;
    velocity.y = 0;
}