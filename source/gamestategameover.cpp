#include "./gamestategameover.h"

#include <cstdio>

GameStateGameOver::GameStateGameOver() {}
GameStateGameOver::~GameStateGameOver() {}

int GameStateGameOver::onStart() { 
    
    scoreLocation.pos = { 117, 5 };
    scoreLocation.scale = 2;
    scoreAnimation.changeAnimatedObject(&scoreLocation);

    scoreTargetLocation.pos = {64.0f, 88.0f};
    scoreTargetLocation.scale = 2;
    scoreAnimation.moveTo(&scoreTargetLocation, 1500);

    scoreAnimation.play();

    return 0; 
}
void GameStateGameOver::onEnd() {}
void GameStateGameOver::onUpdate(float deltaTime) {
    
    scanKeys();
    if(keysDown()&KEY_START)
        exit();

    scoreAnimation.update(deltaTime);

    SpaceObj::MainGameUpdateData dat = { deltaTime };

    for(int a = 0; a < MAX_ASTEROIDS; a++) {
        if(asteroids[a].isAlive()) {
            asteroids[a].onUpdate(&dat);
        }
    }

}
void GameStateGameOver::onDraw(RGNDS::Engine::Screen scr) {
    
    SpaceObj::MainGameDrawData dat = { 0.0, scr };

    for(int a = 0; a < CNT_STARS; a++)
        stars[a].draw();

    for(int a = 0; a < MAX_ASTEROIDS; a++) {
        if(asteroids[a].isAlive()) {
            asteroids[a].onDraw(&dat);
        }
    }

    RGNDS::Transform tra;
    if ( scr == ENGINE_SCREEN_TOP ) {
        scoreAnimation.getLerp(&tra);
        char buffer[16];
        sprintf(buffer, "% 8d", *score);
        RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &tra);
    }
    else {

        tra.pos = { 40, 296 };
        tra.scale = 2;

        RGNDS::GL2D::glText("Press START", 0xffff, &tra, 31, 2);
    } 

}

