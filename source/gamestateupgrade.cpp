#include "gamestateupgrade.h"

#include <stdio.h>

#include "config.h"
#include "nds/arm9/input.h"

#include "../modules/RGNDS_Engine/gl2d.h"
#include "../modules/RGNDS_Engine/nitrofs.h"

#define ERROR_PRICE_TO_HIGH "You don't have enough points"

static int costs[4] = {
    1000, 2000, 3500, 0
};

GameStateUpgrade::GameStateUpgrade(ShipStats *stats) {
    shipstats = stats;
    selection.transform.pos.y += 200;
    selection.transform.pos.x += 8;

    descriptionlocation.pos.x += 8;
    descriptionlocation.pos.y += 34;

    scorelocation.pos += 8;
    scorelocation.scale = 2.0f;

    costlocation.pos.x += 8;
    costlocation.pos.y = 180;
}


int GameStateUpgrade::onStart() {

    description[0] = (char*)RGNDS::Files::loadNitroFS("nitro:/upgrades/shield.txt");
    description[1] = (char*)RGNDS::Files::loadNitroFS("nitro:/upgrades/gencap.txt");
    description[2] = (char*)RGNDS::Files::loadNitroFS("nitro:/upgrades/genreg.txt");
    description[3] = (char*)RGNDS::Files::loadNitroFS("nitro:/upgrades/none.txt");

    selection.addOption("+1 Shield use");
    selection.addOption("Generator Capacity");
    selection.addOption("Generator Regen. -50%");
    selection.addOption("none");

    showError = false;

    return 0;
}

void GameStateUpgrade::onEnd() {
    selection.clearOptions();

    free(description[0]);
    free(description[1]);
    free(description[2]);
    free(description[3]);
}

void GameStateUpgrade::onUpdate(float deltaTime) {
    scanKeys();

    int keys = keysDown();

    if(keys&KEY_DOWN)
        selection.selectNext();

    if(keys&KEY_UP)
        selection.selectPrev();

    if(keys&KEY_A) {
        int selected = selection.selected();


        if(*score >= (costs[selected] * *game_difficulty)) {
            *score -= (costs[selected] * *game_difficulty);
            switch(selected) {
                case 0: // +1 Shielduse
                    shipstats->shielduses += 1;
                    break;

                case 1: // Generator cap +3
                    shipstats->generatorcapacity += 3;
                    break;

                case 2: // Genrator recovery + 50% 
                    shipstats->generatorrecovery *= 1.5f;
                    break;

            }
            exit();
        }
        else {
            showError = true;    
        }
    }

}

void GameStateUpgrade::onDraw(RGNDS::Engine::Screen scr) {
   
    if(scr == ENGINE_SCREEN_BOTTOM) {
        selection.draw();

        RGNDS::GL2D::glRectFilled(
                22, 199 + (10 * selection.selected()), 
                228 , 10,
                Engine_Color16(1, 3, 3, 31),
                12
        );

        if(showError) {
            RGNDS::Transform tra;
            tra.pos.y = 308;
            tra.pos.x = 24;

            RGNDS::GL2D::glText(
                    ERROR_PRICE_TO_HIGH, 
                    Engine_Color16(1, 31, 0, 0),
                    &tra
            );
        }
    }
    else {

        char buffer[16];
        sprintf(buffer, "Score: % 8d", *score);
        RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &scorelocation);

        RGNDS::GL2D::glText(
                description[selection.selected()], 
                0xffff,
                &descriptionlocation
                );
        
        char str[30];
        sprintf(
                str,
                "cost: % 24d", 
                (int)(costs[selection.selected()] * (*game_difficulty))
        );

        RGNDS::GL2D::glText(str, 0xffff, &costlocation);
    }
}
