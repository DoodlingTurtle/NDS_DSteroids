#include "ship.h"

#include <math.h>
#include <maxmod9.h>

#include "config.h"
#include "gamestatemaingame.h"

#include "shipstats.h"
#include "shipupgrade_shield.h"

#include "res/sb1.h"

#define SCREEN_HEIGHT2 384

#define SHIP_DEFAULT_RADIUS 16.0f

static const int COLOR_SHIP = Engine_Color16(1, 31, 0, 0);

static mm_sfxhand thrust_sound;
static float thrustSoundTimer;

ShipExplosionParticle ShipExplosionParticle::proto = ShipExplosionParticle();

ShipExplosionParticle::ShipExplosionParticle() : ParticleSystem::Particle() 
{
    pos.x = Engine_RandF() * 16 - 8;
    pos.y = Engine_RandF() * 16 - 8;

    velocity = Engine_RandF() * 64+32;
    lifetime = Engine_RandF() * 1000+1000;

    directionFromPositionVector();
    
}
ShipExplosionParticle::~ShipExplosionParticle() {}

bool ShipExplosionParticle::update(float deltatime) 
{
    lifetime -= deltatime*1000.0f;
    if(lifetime <= 0) return false;

    moveInDirection(velocity*deltatime);
    velocity *= 0.90;

    return true;
}
void ShipExplosionParticle::attachToVector(float deltaTime, int index, std::vector<RGNDS::Point<double>>* vec) 
{
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
ShipExplosionParticle* ShipExplosionParticle::getNewInstance(int index) 
{
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

void ShipExplosion::onUpdate(SpaceObj::MainGameUpdateData* data)
{
    SpaceObj::updatePosition();
    transform.pos.x = pos.x;
    transform.pos.y = pos.y;

    ParticleSystem::Emitter::update(data->deltaTime);
}
void ShipExplosion::onDraw(SpaceObj::MainGameDrawData* data) 
{
    ParticleSystem::Emitter::draw(COLOR_SHIP, 31, 2);
}
void ShipExplosion::onNoParticlesLeft() { SpaceObj::kill(); }



Ship::Ship() : SpaceObj(SHIP_DEFAULT_RADIUS) 
{

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

    mmLoadEffect(SFX_SFX_LASER1);
    mmLoadEffect(SFX_S_EXP);
    mmLoadEffect(SFX_S_THRUST);

// Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
    reset();
}

Ship::~Ship() 
{
    delete shaBody;
    delete shaThruster;

    clearUpgrades();

    mmUnloadEffect(SFX_SFX_LASER1);
    mmUnloadEffect(SFX_S_EXP);
    mmUnloadEffect(SFX_S_THRUST);
}

void Ship::clearUpgrades() 
{
    currentShield = nullptr;

    for (auto upgrade : upgrades)
        delete upgrade;

    for (auto upgrade : newUpgrades)
        delete upgrade;

    upgrades.clear();
    newUpgrades.clear();
}

void Ship::addUpgrade(ShipUpgrade *upgrade) {
    if(upgrade->init(stats))
        upgrades.push_back(upgrade);
}

void Ship::update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch) {
// Activate new upgrades
    for(int a = 0; a < newUpgrades.size(); a++)
        upgrades.push_back(newUpgrades.at(a));

    newUpgrades.clear();

// Genrator Recovery
    stats->generator += deltaTime * stats->generatorrecovery;
    if(stats->generator > stats->generatorcapacity)
        stats->generator = stats->generatorcapacity;

// Process user Input
    if(keys_held&GameKeyMap[controlls[GAMEINPUT_TURNRIGHT]])
        this->setAngleRel(angRes);

    if(keys_held&GameKeyMap[controlls[GAMEINPUT_TURNLEFT]])
        this->setAngleRel(-angRes);

    if(keys_justpressed&GameKeyMap[controlls[GAMEINPUT_FIRE]]){
        if(stats->generator >= stats->shotenergyconsumption) {
            Shot::Spawn(ang, &pos);
            mmEffect(SFX_SFX_LASER1);

            stats->generator -= stats->shotenergyconsumption;
        }
    }
    
    if(keys_held&GameKeyMap[controlls[GAMEINPUT_ACCELERATE]]) {
        thrusting = true;
        ph.accelerate(deltaTime * 1.5f);
        if(thrust_sound == 0) {
            thrustSoundTimer  = 0.00;
            thrust_sound = mmEffect(SFX_S_THRUST);
        }
        else {
            thrustSoundTimer += 1000.0f * deltaTime;
            if(thrustSoundTimer >= 1000) {
                thrustSoundTimer = 0;
                mmEffectCancel(thrust_sound);
                thrust_sound = mmEffect(SFX_S_THRUST);
            }
        }
    }
    else {
        thrusting = false;
        ph.decerlerate(1);
        mmEffectCancel(thrust_sound);
        thrust_sound = 0;
    }

    ShipUpgrade* upgrade;
    for(int a = upgrades.size()-1; a >= 0; a--) {
        upgrade = upgrades.at(a);
        if(!upgrade->update(stats, this, deltaTime, keys_held, keys_up, keys_justpressed)) {
            Engine_Log("remove upgrade " << upgrade);
            if(upgrade == currentShield) {
                currentShield = nullptr;
                objRadius = SHIP_DEFAULT_RADIUS;
            }

            delete upgrade;
            upgrades.erase(upgrades.begin()+a);
        }
    }

    // Update Position based on physics
    velocity += dir * ph.acceleration;
    
    // Update Position based on Screen-Borders
    updatePosition();

}

void Ship::reset() 
{
    pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    scale = .75;
    setAngle(PI/2);
    velocity.x = 0;
    velocity.y = 0;
    bIsAlive = true;

    clearUpgrades();

    currentShield = new ShipUpgrade_Shield();
    upgrades.push_back(currentShield);  



    objRadius = 24;

    Engine_Log("Generator-Cap: " << stats->generatorcapacity);
}

void Ship::onUpdate(SpaceObj::MainGameUpdateData* dat) 
{
    this->update(
        dat->deltaTime
        , dat->keys_held
        , dat->keys_up
        , dat->keys_justpressed
        , dat->touch
    );
}

void Ship::onDraw(SpaceObj::MainGameDrawData* data) 
{
    SpaceObj::draw([this](Transform* tr) {
        if(this->thrusting)
            this->shaThruster->draw(Engine_Color16(1, 31, 31,  1), tr);

        this->shaBody->draw(COLOR_SHIP, tr, 0, 2);

        for(auto upgrade : upgrades) {
            upgrade->draw(*tr);
        }
    });
   
    int barheight = (int)((stats->generator / stats->generatorcapacity) * 164.0f);
    RGNDS::GL2D::glRectFilled(240, 28 + (164 - barheight), 16, barheight, 0xffff) ;

    RGNDS::GL2D::glRectFilled(240, 192, 16, barheight, 0xffff);
}

bool Ship::gotHit(SpaceObj* culprit) {
    if(currentShield != nullptr) {
        currentShield->gotHit();
        return false;
    }
    else {
        Engine_Log("Ship " << this << " got killed by " << culprit);
        mmEffect(SFX_S_EXP); 
        if(thrust_sound)
            mmEffectCancel(thrust_sound);
        kill(); 
        return true;
    }
}
