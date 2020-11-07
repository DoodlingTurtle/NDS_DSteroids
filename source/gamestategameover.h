#ifndef __GameStateGameOver_H__
#define __GameStateGameOver_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "asteroid.h"
#include "star.h"
#include "config.h"

class GameStateGameOver : public RGNDS::Engine {
public:
    GameStateGameOver();
    virtual~GameStateGameOver();

    Star *stars;
    int *score;
    Asteroid *asteroids;

protected:
    int onStart();
    void onEnd();
    void onUpdate(float deltaTime);
    void onDraw(RGNDS::Engine::Screen screen);

};

#endif