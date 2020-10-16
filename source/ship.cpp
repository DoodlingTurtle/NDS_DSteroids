#include "ship.h"

#define SCREEN_HEIGHT2 384

Ship::Ship() : PolyObj(2, (const PointF[6]){
    {  8.0f,  0.0f }
  , { -4.0f,  0.0f }
  , { -8.0f, -8.0f }

  , {  8.0f,  0.0f }
  , { -4.0f,  0.0f }
  , { -8.0f,  8.0f }
}){

// start broadcasting
    broadcast = new Broadcast(bchShip);

// Initialize the Trhuster (yellow triangle at the bottom of the ship);
    thruster = new RGNDS::Engine::PolyObj(1, (const PointF[3]){
        { -8.0f,  0.0f }
      , { -4.0f, -4.0f }
      , { -4.0f,  4.0f }
    });

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    truePosition = pos;

    renderer.defineWrappingArea(0, SCREEN_WIDTH, SCREEN_HEIGHT2, 0);

}

Ship::~Ship() {
    delete thruster;
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

    RGNDS::EngineGL2D::glStartShape( GL_QUAD );
        glColor( Engine_Color16(1, 16, 31, 4) );
        RGNDS::Point<int> p[4] = {
            { (int)pos.x -8, (int)pos.y -8 } ,
            { (int)pos.x +8, (int)pos.y -8 } ,
            { (int)pos.x +8, (int)pos.y +8 } ,
            { (int)pos.x -8, (int)pos.y +8 }
        };

        RGNDS::EngineGL2D::glSetPoints(4, p);
    RGNDS::EngineGL2D::glEndShape();

    for(int a = 0; a < renderer.getInstanceCnt(); a++) {
        pos = renderer.getInstance(a);

        if(thrusting)
            thruster->draw(0x83ff, screen, this);

        RGNDS::Engine::PolyObj::draw(0x801F, screen);
    }

    pos = truePosition;
    pos.y -= screen;


    //RGNDS::Engine::drawCircle(pos.to<int>(), 16 * scale, 10, Engine_Color16(1, 0, 31, 0));

    pos.y = truePosition.y;
}
