#ifndef __GameStateTitle_H__
#define __GameStateTitle_H__

#include "../modules/RGNDS_Engine/engine.h"

#include "star.h"
#include "asteroid.h"

class GameStateTitle : public RGNDS::Engine {
public:
    GameStateTitle();
    virtual~GameStateTitle();

    Star *stars;
    Asteroid *asteroids;

protected:
    int onStart();
    void onUpdate(float deltaTime);
    void onDraw(RGNDS::Engine::Screen screen);
private:

};

#endif
