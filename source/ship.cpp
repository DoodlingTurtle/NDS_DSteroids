#include "ship.h"

#define SCREEN_HEIGHT2 384

Ship::Ship() : PolyObj(4, (const PointF[4]){
    { -8.0f, -8.0f }
  , { -4.0f,  0.0f }
  , {  8.0f,  0.0f }
  , { -8.0f,  8.0f }
}, Engine_Color16(1, 31, 0, 0), GL_TRIANGLE_STRIP){

// start broadcasting
    broadcast = new Broadcast(bchShip);

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    truePosition = pos;

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);

}

Ship::~Ship() {
    delete broadcast;
}

void Ship::update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch) {

// Process user Input
    if(keys_held&KEY_RIGHT)
        setAngleRel(angRes);

    if(keys_held&KEY_LEFT)
        setAngleRel(-angRes);

    if(keys_held&KEY_X)
        scale += 0.05;

    if(keys_held&KEY_B)
        scale -= 0.05;

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

// Check for WrapAround

    // Update Position based on Screen-Borders
    renderer.updateDrawingInstances(&(this->pos), shipRadius);
    this->truePosition = this->pos;

    broadcast->transmit(bceMove, this);

}

void Ship::draw(int screen) {

    pos.y -= screen;

    RGNDS::Point<int> p[4];

    p[0].x = -8;    p[0].y = -8;
    translate<int, int>(&p[0], &p[0]);

    p[1].x =  8;    p[1].y = -8;
    translate<int, int>(&p[1], &p[1]);

    p[2].x =  8;    p[2].y =  8;
    translate<int, int>(&p[2], &p[2]);

    p[3].x = -8;    p[3].y =  8;
    translate<int, int>(&p[3], &p[3]);

    RGNDS::EngineGL2D::glShape(GL_QUAD, Engine_Color16(1, 16, 31, 4), 4, p);

    pos.y += screen;

    RGNDS::Point<float> posOrig = pos;
    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        pos = renderer.getInstance(a);
        pos.y -= screen;

        if(thrusting) {
            p[0].x = -8;    p[0].y =  0;
            translate<int, int>(&p[0], &p[0]);

            p[1].x = -4;    p[1].y = -4;
            translate<int, int>(&p[1], &p[1]);

            p[2].x = -4;    p[2].y =  4;
            translate<int, int>(&p[2], &p[2]);

            RGNDS::EngineGL2D::glShape(GL_TRIANGLE, Engine_Color16(1, 31, 31, 0), 3, p);
        }

        RGNDS::Engine::PolyObj::draw(0);

    }
    pos = posOrig;
}
