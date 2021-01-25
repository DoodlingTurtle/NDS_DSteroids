#include "gamestateupgrade.h"
#include "config.h"
#include "nds/arm9/input.h"

GameStateUpgrade::GameStateUpgrade(ShipStats *stats) {
    shipstats = stats;
}


int GameStateUpgrade::onStart() {
    
    selection = new RGNDS::TextMenu();

    selection->addOption("+ 1 Shield use");
    selection->addOption("Generator Capacity + 3");
    selection->addOption("Generator Regen. -50%");

    return 0;
}


void GameStateUpgrade::onUpdate(float deltaTime) {
    scanKeys();

    int keys = keysDown();

    if(keys&KEY_DOWN)
        selection->selectNext();

    if(keys&KEY_UP)
        selection->selectPrev();

    if(keys&KEY_A) {
        switch(selection->selected()) {
            case 1: // Generator cap +3
                shipstats->generatorcapacity += 3;
                break;
            case 2: 
                shipstats->generatorrecovery *= 2;


        }
        exit();
    }
    

}

void GameStateUpgrade::onDraw(RGNDS::Engine::Screen scr) {

    selection->draw();

}
