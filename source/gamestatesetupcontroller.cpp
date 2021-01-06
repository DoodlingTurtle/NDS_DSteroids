#include "gamestatesetupcontroller.h"
#include "nds/arm9/input.h"
#include "nds/arm9/videoGL.h"
#include "nds/input.h"

#include "nds.h"
#include "filesystem.h"

#include "res/dsc_bg.h"
#include "res/dsc_up.h"
#include "res/dsc_down.h"
#include "res/dsc_left.h"
#include "res/dsc_right.h"
#include "res/dsc_select.h"
#include "res/dsc_l.h"


RGNDS::Transform GameStateSetupController::keyGfxLocation[11] = {
    RGNDS::Transform(74, 98),   //LEFT
    RGNDS::Transform(74, 66),   //RIGHT
    RGNDS::Transform(74, 56),   //UP
    RGNDS::Transform(74, 88),   //DOWN
    RGNDS::Transform(8, 24),    //L
    RGNDS::Transform(176, 102), //Y
    RGNDS::Transform(176, 70),  //A
    RGNDS::Transform(176, 60),  //X
    RGNDS::Transform(176, 92),  //B
    RGNDS::Transform(160, 24),  //R
    RGNDS::Transform(160, 151)   //SELECT
};

char* GameStateSetupController::keyLabels[NUM_INPUT_BUTTONS] = {
    (char*)"Accelerate",
    (char*)"Turn left",
    (char*)"Turn right",
    (char*)"Fire"
};


GameStateSetupController::GameStateSetupController() {
    for(int a = 0; a < NUM_INPUT_BUTTONS; a++) 
        menu.addOption(keyLabels[a]);

    menu.transform.pos.x = 8;
    menu.transform.pos.y = 192;
    menu.transform.scale = 1;

    backbtnlocation.pos.y = 368;
    backbtnlocation.pos.x = 8;
   
}
GameStateSetupController::~GameStateSetupController() {}

static void loadSprite(u16* bgPalette, int texSize, const char* texFile, glImage* target) {

    FILE* f1 = fopen(texFile, "rb");
    u8* bgTexture = (u8*)valloc(texSize+1);
    fread(bgTexture, texSize, 1, f1);
    fclose(f1);

    RGNDS::GL2D::glLoadTileSet(
            target, 
            256, 128, 256, 128, 
            GL_RGB256, 
            TEXTURE_SIZE_256, TEXTURE_SIZE_128, 
            GL_TEXTURE_COLOR0_TRANSPARENT, 2, 
            bgPalette, 
            bgTexture
    );

    free(bgTexture);
}

int GameStateSetupController::onStart() { 

    RGNDS::TextMenu* tm;
    int a;

    for(a = 0; a < NUM_INPUT_BUTTONS; a++) { 
        tm = new RGNDS::TextMenu(RGNDS::TEXTMENU_SINGLE_HORIZONTAL);
    
        tm->addOption("LEFT");
        tm->addOption("RIGHT");
        tm->addOption("UP");
        tm->addOption("DOWN");
        tm->addOption("L");
        tm->addOption("Y");
        tm->addOption("A");
        tm->addOption("X");
        tm->addOption("B");
        tm->addOption("R");
        tm->addOption("SELECT");

        tm->transform.pos.y = 192 + 10 * a;
        tm->transform.pos.x = SCREEN_WIDTH - tm->getPXWidth() - 8;    
        tm->setSelection(keys[a]);

        controls.push_back(tm);

    }

    FILE* f = fopen(_DSC_BG_PAL_FILE, "rb");
    u16* bgPalette = (u16*)valloc(_DSC_BG_PAL_SIZE);   
    fread(bgPalette, _DSC_BG_PAL_SIZE, 1, f);
    fclose(f);
   
    loadSprite(bgPalette, _DSC_LEFT_SIZE, _DSC_LEFT_FILE, &(bgTopScreen[0]));
    loadSprite(bgPalette, _DSC_RIGHT_SIZE, _DSC_RIGHT_FILE, &(bgTopScreen[1]));
    loadSprite(bgPalette, _DSC_UP_SIZE, _DSC_UP_FILE, &(bgTopScreen[2]));
    loadSprite(bgPalette, _DSC_DOWN_SIZE, _DSC_DOWN_FILE, &(bgTopScreen[3]));
    loadSprite(bgPalette, _DSC_L_SIZE, _DSC_L_FILE, &(bgTopScreen[4]));
    loadSprite(bgPalette, _DSC_SELECT_SIZE, _DSC_SELECT_FILE, &(bgTopScreen[5]));
    loadSprite(bgPalette, _DSC_BG_SIZE, _DSC_BG_FILE, &(bgTopScreen[6]));

    free(bgPalette);

    return 0; 
}
void GameStateSetupController::onEnd() {
    for(unsigned int a = 0; a < controls.size(); a++) {
        keys[a] = controls.at(a)->selected();
        delete controls.at(a);
    }

    for(int a = 0; a < 7; a++) 
        RGNDS::GL2D::glFreeTileSet(&(bgTopScreen[a]));
   
    Engine_Log("Accelerate key: " << keys[0]);
    Engine_Log("Turn Right key: " << keys[1]);
    Engine_Log("Turn Left key: " << keys[2]);
    Engine_Log("Fire key: " << keys[3]);

    controls.clear();
}

void GameStateSetupController::onUpdate(float deltaTime) {
    scanKeys();
    unsigned int press = keysDown();


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

        RGNDS::GL2D::glRectFilled(
                22, 191 + (10 * menu.selected()), 
                220 , 10,
                Engine_Color16(1, 3, 3, 31),
                12
        );

        menu.draw();

        for(auto txt : controls)
            txt->draw();

        RGNDS::GL2D::glText("B = Back", 0xffff, &backbtnlocation);
    }
    else {
    
        RGNDS::Transform tr(0, 32);
        int sel;

        glColor(Engine_Color16(1, 12, 12, 12));
        RGNDS::GL2D::glSprite(0, &bgTopScreen[6], &tr);

        for(int a = 0; a < controls.size(); a++) {
            sel =  controls.at(a)->selected();

            tr.pos.y = 32;
            tr.pos.x = (sel > 4 && sel < 10) ? 140 : 0;

            glColor(Engine_Color16(1, 20, 20, 20));
            RGNDS::GL2D::glSprite(0, &bgTopScreen[(sel < 10) ? (sel%5) : 5], &tr);

            tr.pos = keyGfxLocation[sel].pos;


            glColor(0xffff);
            RGNDS::GL2D::glText(
                keyLabels[a],
                0xffff, 
                &tr
            );  
        }
    }
}
