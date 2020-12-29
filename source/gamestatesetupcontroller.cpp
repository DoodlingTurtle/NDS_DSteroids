#include "gamestatesetupcontroller.h"
#include "nds/arm9/videoGL.h"
#include "nds/input.h"

#include "nds.h"
#include "filesystem.h"
#include "res/DSControlls.h"

GameStateSetupController::GameStateSetupController() {
    menu.addOption((char*)"Accelerate");
    menu.addOption((char*)"Turn right");
    menu.addOption((char*)"Turn left");
    menu.addOption((char*)"Fire");

    menu.transform.pos.x = 8;
    menu.transform.pos.y = 8 + SCREEN_HEIGHT;
    menu.transform.scale = 1;

    backbtnlocation.pos.y = 368;
    backbtnlocation.pos.x = 8;
   
}
GameStateSetupController::~GameStateSetupController() {}


int GameStateSetupController::onStart() { 

    RGNDS::TextMenu* tm;
    int a;

    for(a = 0; a < 4; a++) { 
        tm = new RGNDS::TextMenu(RGNDS::TEXTMENU_SINGLE_HORIZONTAL);
    
        tm->addOption("LEFT");
        tm->addOption("RIGHT");
        tm->addOption("UP");
        tm->addOption("DOWN");
        tm->addOption("SELECT");
        tm->addOption("A");
        tm->addOption("B");
        tm->addOption("X");
        tm->addOption("Y");
        tm->addOption("L");
        tm->addOption("R");

        tm->transform.pos.y = 200 + 8 * a;
        tm->transform.pos.x = SCREEN_WIDTH - tm->getPXWidth() - 8;    
        tm->setSelection(keys[a]);

        controls.push_back(tm);

    }

    Engine_Log("load pal");
    FILE* f = fopen(_DSCONTROLLS_PAL_FILE, "rb");
    bgPalette = (u16*)valloc(_DSCONTROLLS_PAL_SIZE+1);   
    fread(bgPalette, _DSCONTROLLS_PAL_SIZE, 1, f);
    fclose(f);

    Engine_Log("load tex");
    FILE* f1 = fopen(_DSCONTROLLS_FILE, "rb");
    bgTexture = (u8*)valloc(_DSCONTROLLS_SIZE+1);
    fread(bgTexture, _DSCONTROLLS_SIZE, 1, f1);
    fclose(f1);

    RGNDS::GL2D::glLoadTileSet(
            bgTopScreen, 
            256, 192, 256, 192, 
            GL_RGB256, 
            TEXTURE_SIZE_256, TEXTURE_SIZE_256, 
            0, 2, 
            bgPalette, 
            bgTexture
    );

    return 0; 
}
void GameStateSetupController::onEnd() {
    for(unsigned int a = 0; a < controls.size(); a++) {
        keys[a] = controls.at(a)->selected();
        delete controls.at(a);
    }

    controls.clear();

    RGNDS::GL2D::glFreeTileSet(bgTopScreen);
    free(bgTexture);
    free(bgPalette);
}

void GameStateSetupController::onUpdate(float deltaTime) {
    scanKeys();
    int press = keysDown();

    if(press&KEY_DOWN)
        menu.selectNext();

    else if(press&KEY_UP)
        menu.selectPrev();
   
    else if(press&KEY_RIGHT)
        controls.at(menu.selected())->selectNext();

    else if(press&KEY_LEFT)
        controls.at(menu.selected())->selectPrev();

    else if(press&KEY_B)
        exit();
}

void GameStateSetupController::onDraw(RGNDS::Engine::Screen screen) {
    if(screen == ENGINE_SCREEN_BOTTOM) {
        menu.draw();

        for(auto txt : controls)
            txt->draw();

        RGNDS::GL2D::glText("B = Back", 0xffff, &backbtnlocation);
    }
    else {
        RGNDS::GL2D::glSprite(0, bgTopScreen);
    }
}
