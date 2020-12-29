#ifndef __GameStateSetupController_H__
#define __GameStateSetupController_H__

#include <vector>

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"

#include "config.h"

class GameStateSetupController : public RGNDS::Engine {
public:
    GameStateSetupController();
    virtual~GameStateSetupController();
    
    int* keys;

protected:
    int onStart();
     
    void onUpdate(float deltaTime);
    void onDraw(RGNDS::Engine::Screen screen);
    void onEnd();

private:
    RGNDS::TextMenu menu;    
    RGNDS::Transform backbtnlocation;
    
    std::vector<RGNDS::TextMenu*> controls;

    glImage bgTopScreen[1]; 


    u16* bgPalette;
    u8* bgTexture;

};

#endif
