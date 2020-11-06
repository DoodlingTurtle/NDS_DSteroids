#include "ship.h"
#include <math.h>

#include "gamestatemaingame.h"
#include "broadcastchannels.h"

#define SCREEN_HEIGHT2 384

static const int COLOR_SHIP = Engine_Color16(1, 31, 0, 0);


ShipExplosionParticle ShipExplosionParticle::proto = ShipExplosionParticle();

ShipExplosionParticle::ShipExplosionParticle() : ParticleSystem::Particle() {
    pos.x = Engine_RandF() * 16 - 8;
    pos.y = Engine_RandF() * 16 - 8;

    velocity = Engine_RandF() * 4+3;
    lifetime = Engine_RandF() * 1000+1000;

    directionFromPositionVector();
    
}
ShipExplosionParticle::~ShipExplosionParticle() {}

bool ShipExplosionParticle::update(float deltatime) {
    lifetime -= deltatime*1000.0f;
    if(lifetime <= 0) return false;

    moveInDirection(velocity*deltatime);

    return true;
}

void ShipExplosionParticle::attachToVector(float deltaTime, int index, std::vector<RGNDS::Point<double>>* vec) {
    RGNDS::Point<double> d;
    d.x = pos.x-1;
    d.y = pos.y-1;

    vec->push_back(d);
    
    d.x += 3;
    vec->push_back(d);
    d.x -= 3;

    d.y += 3;
    vec->push_back(d);
}

ShipExplosionParticle* ShipExplosionParticle::getNewInstance(int index) {
    return new ShipExplosionParticle();
}



ShipExplosion::ShipExplosion(Ship* ship) : ParticleSystem::Emitter(ship->pos.x, ship->pos.y, 20, &ShipExplosionParticle::proto)
{
    velocity.x = ship->velocity.x / 4;
    velocity.y = ship->velocity.y / 4;
    pos.x = transform.pos.x;
    pos.y = transform.pos.y;

    bIsAlive = true;
}
ShipExplosion::~ShipExplosion(){};

void ShipExplosion::onUpdate(SpaceObj::MainGameUpdateData* data){
    SpaceObj::updatePosition();
    transform.pos.x = pos.x;
    transform.pos.y = pos.y;

    ParticleSystem::Emitter::update(data->deltaTime);
}
void ShipExplosion::onDraw(SpaceObj::MainGameDrawData* data) {
    ParticleSystem::Emitter::draw(COLOR_SHIP, 31, 2);
}
void ShipExplosion::onNoParticlesLeft() { SpaceObj::kill(); }



Ship::Ship() : SpaceObj(16.0f) {

    shaBody = new RGNDS::GL2D::PolyShape(
          4,
          (const RGNDS::Point<double>[4]){
                { -16, -16 }
              , { - 8,   0 }
              , {  16,   0 }
              , { -16,  16 }
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

    this->bIsAlive = true;

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    reset();
}

Ship::~Ship() {
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

    if(keys_justpressed&(KEY_R|KEY_L))
        Shot::Spawn(ang, &pos);
    
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

}

void Ship::reset() {
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    scale = .75;
    setAngle(PI/2);
    velocity.x = 0;
    velocity.y = 0;
    bIsAlive = true;
}

void Ship::onUpdate(SpaceObj::MainGameUpdateData* dat) {
    this->update(
        dat->deltaTime
        , dat->keys_held
        , dat->keys_up
        , dat->keys_justpressed
        , dat->touch
    );
}

void Ship::onDraw(SpaceObj::MainGameDrawData* data) {
    SpaceObj::draw([this](Transform* tr) {
        if(this->thrusting)
            this->shaThruster->draw(Engine_Color16(1, 31, 31,  1), tr);

        this->shaBody->draw(COLOR_SHIP, tr, 0, 2);
    });
}