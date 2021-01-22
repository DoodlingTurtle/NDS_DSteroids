#ifndef __GameStateCredits_H__
#define __GameStateCredits_H__

#include "../modules/RGNDS_Engine/engine.h"

class GameStateCredits : public RGNDS::Engine {
public:
    GameStateCredits();
    virtual~GameStateCredits();

    void onDraw(RGNDS::Engine::Screen scr);

protected:
    int onStart();

    void onUpdate(float deltaTime);

    void onEnd();


private:
    char* creditsText;


};

#endif
