#ifndef __GameStatePause_H__
#define __GameStatePause_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"

class GameStatePause : public RGNDS::Engine {
public:
    GameStatePause(RGNDS::Engine* backgroundProvider);
    virtual~GameStatePause();

    void onDraw(RGNDS::Engine::Screen scr);

    bool endGame();

protected:

    int onStart(); 
    void onUpdate(float deltaTime);

private:
    RGNDS::TextMenu textMenu;
    RGNDS::Engine* backgroundProvider; 

};

#endif
