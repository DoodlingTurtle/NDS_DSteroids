#include "asteroid.h"

#include "../modules/RGNDS_Engine/engine.h"

#include "gamestatemaingame.h"
#include "ship.h"

#include <math.h>

#include "collision.h"

#define SCREEN_HEIGHT2 384
#define RandF() ((float)rand() / (float)RAND_MAX)


static const int COLOR_ASTEROIDS = Engine_Color16(1, 14, 11, 10);


std::vector<Shot*> Asteroid::shots;

Ship* Asteroid::ship = nullptr;

float AsteroidParticle::scale = 1.0f;

AsteroidParticle AsteroidParticle::proto = AsteroidParticle();

AsteroidParticle::~AsteroidParticle(){}

AsteroidParticle::AsteroidParticle() {
    pos.x = (Engine_RandF() * 8 - 4) * scale;
    pos.y = (Engine_RandF() * 8 - 4) * scale;

    directionFromPositionVector();

    lifetime = 2000;
}

AsteroidParticle* AsteroidParticle::getNewInstance(int index) { return new AsteroidParticle(); }

bool AsteroidParticle::update(float deltaTime) {
    lifetime -= deltaTime * 1000.0;
    if(lifetime <= 0) return false;

    moveInDirection(6 * deltaTime);

    return true;
}

void AsteroidParticle::attachToVector(float deltaTime, int index, std::vector<RGNDS::Point<double>>* vec) {

    RGNDS::Point<double> d;
    d.x = pos.x;
    d.y = pos.y;

    vec->push_back(d);
    
    d.x += 2;
    vec->push_back(d);
    d.x -= 2;

    d.y += 2;
    vec->push_back(d);

}


AsteroidExplosion::AsteroidExplosion(float x, float y) 
    : ParticleSystem::Emitter::Emitter(x, y, 5, &AsteroidParticle::proto) 
    , SpaceObj(16)
{}

void AsteroidExplosion::onUpdate(SpaceObj::MainGameUpdateData* data) { 
    update(data->deltaTime);
 }

void AsteroidExplosion::onDraw(SpaceObj::MainGameDrawData* data) {
    ParticleSystem::Emitter::draw(COLOR_ASTEROIDS, 31, 0);
}

void AsteroidExplosion::revive(float x, float y, float scale) {
   
   
    Engine_Log("revive particles" << scale);
    AsteroidParticle::scale = scale;
    spawnNewParticles(5*scale);
    AsteroidParticle::scale = 1;

    this->transform.pos.x = x;
    this->transform.pos.y = y;
    this->bIsAlive = true;
}

void AsteroidExplosion::onNoParticlesLeft() {
    Engine_Log("No Particles left");
    this->kill();
}




Asteroid::Asteroid() : 
    RGNDS::GL2D::PolyShape(18, nullptr, GL_QUAD_STRIP) 
    , SpaceObj(16.0f)    
{
    generateShape();

    bIsAlive = false;
}

void Asteroid::generateShape() {
    float angSteps = PI2 / ((numPoints-2) / 2);
    float radius = 16;
    float ang = 0;
    int a;

    // Setup Shape
    float pointdist[numPoints + 1];

    for(a = 0; a < (numPoints-2)/2 + 1; a++){
        pointdist[a] = radius
            + ((RandF() * 8 - 4) * (a%2 > 0));
    }

    this->setPoint(0, 0, 0);
    this->setPoint(1, radius, 0);

    for(a = 0; a < numPoints; a+=2) {
        ang += angSteps;

        this->setPoint(a, 0, 0);
        this->setPoint(
            a+1,
            cos(ang) * pointdist[a/2],
            sin(ang) * pointdist[a/2]
        );
    }

    return;
}

void Asteroid::bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale) {
    Engine_Log("Bring asteroid back to life");

    setAngle(RandF() * PI2);
    velocity.x = (RandF() * 2) - 1;
    velocity.y = (RandF() * 2) - 1;
    spinSpeed = (RandF() * 0.5 + 0.5) * 0.0625; // Spin by 360� every 16 Seconds (at max spinspeed)

    this->pos.x = pos.x;
	this->pos.y = pos.y;
    this->scale = scale;

    if(generateNewShape)
        generateShape();

    bIsAlive = true;
}

void Asteroid::onDraw(SpaceObj::MainGameDrawData* data) {
    if(
        pos.y < 16 || pos.y > SCREEN_HEIGHT2 - 16
        ||
        (((MainGameDrawData*)data)->screen == 0 && pos.y < SCREEN_HEIGHT+16)
        ||
        (((MainGameDrawData*)data)->screen == 1 && pos.y > SCREEN_HEIGHT-16)
    ) {
        SpaceObj::draw([this](RGNDS::Transform* tr){
            RGNDS::GL2D::PolyShape::draw(COLOR_ASTEROIDS, tr);
        });
    }
}

void Asteroid::onUpdate(SpaceObj::MainGameUpdateData* data) {
    float deltatime = data->deltaTime;

    setAngleRel(PI2 * (deltatime * spinSpeed));
    updatePosition();

    if(ship != nullptr) {
        RGNDS::Point <float> p;
        float r;
        ship->getCollisionSphere(&p, &r);

        if(RGNDS::Collision::checkCircleOnCircle(
            &this->pos, this->scale * 14,
            &p, r
        )) {
            Engine_Log("Asteroid " << this << " hit ship " << ship);
            ship->kill();

            return;
        }

        for(Shot* s : Shot::getShots()) {
            if(RGNDS::Collision::checkCircleOnCircle(
                &this->pos
              , this->scale * 14
              , &s->pos, 2
            )) {
                s->kill();
                this->kill();
            }
        }
    }
}

short Asteroid::getScoreValue() {
    return 100/scale;
}

Asteroid::~Asteroid() {
}
