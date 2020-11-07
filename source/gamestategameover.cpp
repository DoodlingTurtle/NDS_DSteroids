#include "./gamestategameover.h"

GameStateGameOver::GameStateGameOver() {}
GameStateGameOver::~GameStateGameOver() {}

int GameStateGameOver::onStart() { return 0; }
void GameStateGameOver::onEnd() {}
void GameStateGameOver::onUpdate(float deltaTime) {
    
    scanKeys();
    if(keysDown()&KEY_START)
        exit();


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

    if ( scr == ENGINE_SCREEN_BOTTOM ) {

        RGNDS::Transform tra;

        tra.pos.x = (SCREEN_WIDTH-176)/2;
        tra.pos.y = 192*1.5 - 8 ;
        tra.scale = 2;

        RGNDS::GL2D::glText("Press START", 0xffff, &tra, 31, 2);
    } 

}
