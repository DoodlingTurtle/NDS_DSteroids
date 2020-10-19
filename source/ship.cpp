#include "ship.h"

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

    scale = .75;

// start broadcasting
    broadcast = new Broadcast(bchShip);

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);
}

Ship::~Ship() {
    delete broadcast;
    delete shaBody;
    delete shaThruster;
}

void Ship::update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch) {

// Process user Input
    if(keys_held&KEY_RIGHT)
        this->setAngleRel(angRes);

    if(keys_held&KEY_LEFT)
        this->setAngleRel(-angRes);

    if(keys_held&KEY_X)
        this->scale += 0.05;

    if(keys_held&KEY_B)
        this->scale -= 0.05;

    if(keys_held&KEY_UP) {
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
    float shipRadius = 8 * scale;

    // Update Position based on Screen-Borders
    renderer.updateDrawingInstances(&(this->pos), shipRadius);

    broadcast->transmit(bceMove, this);
}

void Ship::draw(int screen) {
    RGNDS::GL2D::PolyShape *c = RGNDS::GL2D::PolyShape::createCircle(16, 8, 1);
    c->draw(Engine_Color16(1, 12, 21, 31), screen, this);
    delete c;

    RGNDS::Point<float> posOrig = this->pos;

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        this->pos = renderer.getInstance(a);
        if(thrusting)
            shaThruster->draw(Engine_Color16(a, 31, 31,  0), screen, this);

        shaBody->draw(Engine_Color16(a, 31,  0,  0), screen, this);
    }

    this->pos = posOrig;
}
