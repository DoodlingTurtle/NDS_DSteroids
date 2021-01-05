#include "gamestatepause.h"
#include "../modules/RGNDS_Engine/gl2d.h"
#include "nds/arm9/video.h"

GameStatePause::GameStatePause(RGNDS::Engine* backgroundProvider) {
    this->backgroundProvider = backgroundProvider;

    textMenu.transform.pos.x = 64;
    textMenu.transform.pos.y = 80;
    textMenu.transform.scale = 2.0f;
    textMenu.zDepth =7;
     
    textMenu.addOption("Resume");
    textMenu.addOption("Exit");
}

GameStatePause::~GameStatePause() {}

int GameStatePause::onStart() { return 0; }

void GameStatePause::onUpdate(float deltaTime) {

    scanKeys();
    int input = keysDown();

    if(input&KEY_DOWN)
        textMenu.selectNext();

    if(input&KEY_UP)
        textMenu.selectPrev();

    if(input&KEY_A)
        exit();

}

void GameStatePause::onDraw(RGNDS::Engine::Screen scr) {
    this->backgroundProvider->onDraw(scr);
    RGNDS::GL2D::glRectFilled(0, 0, 256, 384, 0x0000, 15, 4);

    if(scr != ENGINE_SCREEN_TOP) 
        return;

    RGNDS::GL2D::glRectFilled(
            60, 78 + (18 * textMenu.selected()), 
            134 ,20,
            Engine_Color16(1, 3, 3, 31),
            12,  5
    );
    textMenu.draw();
}

bool GameStatePause::endGame() {
    return textMenu.selected() == 1;
}
