#ifndef __GameStateTitle_H__
#define __GameStateTitle_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "star.h"
#include "asteroid.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"

class GameStateTitle : public RGNDS::Engine {
public:
    GameStateTitle();
    virtual~GameStateTitle();

    Star *stars;
    Asteroid *asteroids;

    int selected();

protected:
    int onStart();
    void onUpdate(float deltaTime);
    void onDraw(RGNDS::Engine::Screen screen);

private:
    RGNDS::TextMenu menu;
    RGNDS::Transform titlelocation;
};

#endif
