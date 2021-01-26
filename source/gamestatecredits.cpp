#include "gamestatecredits.h"

#include "../modules/RGNDS_Engine/gl2d.h"
#include "../modules/RGNDS_Engine/nitrofs.h"


GameStateCredits::GameStateCredits() {}
GameStateCredits::~GameStateCredits() {}

int GameStateCredits::onStart() {

    creditsText = (char*)RGNDS::Files::loadNitroFS(155, "nitro:/credits.txt");  
    Engine_Log("Credits Text: " << creditsText);

    return 0;
}

void GameStateCredits::onUpdate(float deltaTime) {
 
    scanKeys();
    int press = keysDown();


    if(press&KEY_B)
        exit();


}

void GameStateCredits::onDraw(RGNDS::Engine::Screen scr) {

    if(scr == ENGINE_SCREEN_TOP) {
        RGNDS::Transform tra(64, 88);
        tra.scale = 2.0f;

        RGNDS::GL2D::glText("Credits:", 0xffff, &tra); 
    }
    else {
        RGNDS::Transform tra(8, 200);
        RGNDS::GL2D::glText(creditsText, 0xffff, &tra);
        
        tra.pos.y = 368;
        RGNDS::GL2D::glText("B = back", 0xffff, &tra);
    }

}

void GameStateCredits::onEnd() {
    free(creditsText);
}
