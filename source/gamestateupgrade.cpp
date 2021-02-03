#include "gamestateupgrade.h"

#include <stdio.h>
#include <functional>

#include "config.h"
#include <nds/arm9/input.h>

#include "../modules/RGNDS_Engine/gl2d.h"
#include "../modules/RGNDS_Engine/nitrofs.h"

#define ERROR_PRICE_TO_HIGH "You don't have enough points"

#define NUM_TOTAL_UPGRADES 4

static int costs[NUM_TOTAL_UPGRADES] = {
       0,
    1000, 
    2050, 
    3500 
};
static const char* labels[NUM_TOTAL_UPGRADES] = {
    "none",
    "+1 Shield use",
    "Generator capacity up",
    "Generator speed + 50%"
};

static const char* files[NUM_TOTAL_UPGRADES] = {
    "nitro:/upgrades/none.txt",
    "nitro:/upgrades/shield.txt",
    "nitro:/upgrades/gencap.txt",
    "nitro:/upgrades/genreg.txt"
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

};

int GameStateUpgrade::onStart() {


// Shield is always available    
    upgrade_data.push_back(1);

// Randomize the other upgrades        
// TODO: (DoTu) add more upgrade options
    int rnd = 2 + (Engine_RandF()*2.0f);
    upgrade_data.push_back(rnd);

// Append the None Option
    upgrade_data.push_back(0);

    for(auto u : upgrade_data) {
        selection.addOption(labels[u]);
        description.push_back((char*)RGNDS::Files::loadNitroFS(files[u]));
    }



    showError = false;

    return 0;
}

void GameStateUpgrade::onEnd() {
    upgrade_data.clear();
    for(char* u : description)
        free(u);
    

    description.clear();

    selection.clearOptions();
}

void GameStateUpgrade::onUpdate(float deltaTime) {
    scanKeys();

    int keys = keysDown();

    if(keys&KEY_DOWN)
        selection.selectNext();

    if(keys&KEY_UP)
        selection.selectPrev();

    if(keys&KEY_A) {
        int selected = upgrade_data.at(selection.selected());
        int cost = costs[selected];

        if(*score >= cost) {
            *score -= cost;

            switch(selected) {
                case 0: // none 
                    break;

                case 1: // shielduses 
                    shipstats->shielduses += 1;
                    break;

                case 2: // Generator capacity 
                    shipstats->generatorcapacity += 3;
                    break;

                case 3: // Generator regen
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
        
        char str[31];
        sprintf(
                str,
                "cost: % 24d", 
                (int)(costs[upgrade_data.at(selection.selected())] * (*game_difficulty))
        );

        RGNDS::GL2D::glText(str, 0xffff, &costlocation);
    }
}
