#ifndef __GameStateSetupController_H__
#define __GameStateSetupController_H__

#include <vector>

#include "../modules/RGNDS_Engine/transform.h"
#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"

#include "config.h"

#define NUM_INPUT_BUTTONS 4

class GameStateSetupController : public RGNDS::Engine {
public:
    GameStateSetupController();
    virtual~GameStateSetupController();
    
    int* keys;

    void onDraw(RGNDS::Engine::Screen screen);

protected:
    int onStart();
     
    void onUpdate(float deltaTime);
    void onEnd();

private:
    RGNDS::TextMenu menu;    
    RGNDS::Transform backbtnlocation;
    
    std::vector<RGNDS::TextMenu*> controls;

    glImage bgTopScreen[7]; 


    u16* bgPalette;
    u8* bgTexture;

    static RGNDS::Transform keyGfxLocation[11];
    static char* keyLabels[NUM_INPUT_BUTTONS];
};

#endif
