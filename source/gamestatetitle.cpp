#include "gamestatetitle.h"
#include "config.h"

#include "spaceobj.h"
GameStateTitle::GameStateTitle() {}
GameStateTitle::~GameStateTitle() {}

int GameStateTitle::onStart() {
    return 0;
}

void GameStateTitle::onUpdate(float deltaTime) {

    SpaceObj::MainGameUpdateData dat = { deltaTime };

    for(int a = 0; a < MAX_ASTEROIDS; a++) {
        if(asteroids[a].isAlive()) {
            asteroids[a].onUpdate(&dat);
        }
    }
}
void GameStateTitle::onDraw(RGNDS::Engine::Screen screen) {

    SpaceObj::MainGameDrawData dat = { 0.0, screen };

    for(int a = 0; a < CNT_STARS; a++)
        stars[a].draw();

    for(int a = 0; a < MAX_ASTEROIDS; a++) {
        if(asteroids[a].isAlive()) {
            asteroids[a].onDraw(&dat);
        }
    }

}