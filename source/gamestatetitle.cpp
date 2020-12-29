#include "gamestatetitle.h"
#include "config.h"

#include "spaceobj.h"
GameStateTitle::GameStateTitle() : RGNDS::Engine() {

    menu.transform.pos.y = 272;
    menu.transform.pos.x = 32;

    menu.transform.scale = 2;

    titlelocation.pos.x = 20;
    titlelocation.pos.y = 84;
    titlelocation.scale = 3;

    this->menu.addOption((char*)"new game");
    this->menu.addOption((char*)"options");
}
GameStateTitle::~GameStateTitle() {}


int GameStateTitle::onStart() {
    return 0;
}

void GameStateTitle::onUpdate(float deltaTime) {

    SpaceObj::MainGameUpdateData dat = { deltaTime };

    for(int a = 0; a < MAX_ASTEROIDS; a++)
        if(asteroids[a].isAlive())
            asteroids[a].onUpdate(&dat);

    scanKeys();
    int press = keysDown();

    if(press&KEY_DOWN)
        menu.selectNext();
    else if(press&KEY_UP)
        menu.selectPrev();
    else if(press&(KEY_START|KEY_A))
        exit();

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

    if(screen == ENGINE_SCREEN_BOTTOM)
        menu.draw();
    
    if(screen == ENGINE_SCREEN_TOP)
        RGNDS::GL2D::glText("DSTeroids", 0xffff, &titlelocation, 31, 2);
}

int GameStateTitle::selected() {
    return menu.selected();
}
