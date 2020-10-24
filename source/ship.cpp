#include "ship.h"
#include <math.h>

#define SCREEN_HEIGHT2 384

Ship::Ship() : SpaceObj(16.0f) {

    shaBody = new RGNDS::GL2D::PolyShape(
          4,
          (const RGNDS::Point<double>[4]){
                { -16.0f, -16.0f }
              , { - 8.0f,   0.0f }
              , {  16.0f,   0.0f }
              , { -16.0f,  16.0f }
          },
          GL_TRIANGLE_STRIP
    );

    shaThruster = new RGNDS::GL2D::PolyShape(
         3,
         (const RGNDS::Point<double>[3]){
              { -16,   0 },
              { - 8,  -8 },
              { - 8,   8 },
         },
         GL_TRIANGLE
    );
    
// start broadcasting
    broadcast = new Broadcast(bchShip);

    shots = new std::vector<Shot*>();

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    reset();
}

Ship::~Ship() {
    delete broadcast;
    delete shaBody;
    delete shaThruster;

    for(auto shot : *shots)
        delete shot;

    shots->clear();
    delete shots;
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

    if(keys_justpressed&(KEY_R|KEY_L))
        shots->push_back(new Shot(ang, &pos));
    
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
    
    // Update Position based on Screen-Borders
    updatePosition();
    broadcast->transmit(bceMove, this);

    // Process Shots
    std::vector<Shot*> *nextRound = new std::vector<Shot*>();
    
    for(auto shot : *shots) {
        if(shot->update(deltaTime))
            nextRound->push_back(shot);
    }

    shots->clear();
    delete shots;
    shots = nextRound;

}

void Ship::draw() {
    SpaceObj::draw([this](Transform* tr) {
        if(this->thrusting)
            this->shaThruster->draw(Engine_Color16(1, 31, 31,  0), tr);

        this->shaBody->draw(Engine_Color16(1, 31,  0,  0), tr);
    });

    for(auto shot : *shots) {
        shot->draw();
    }
}

void Ship::reset() {
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    scale = .75;
    setAngle(PI/2);
    velocity.x = 0;
    velocity.y = 0;
}