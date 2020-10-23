/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>

#include <nds.h>
#include <time.h>

#include "../modules/RGNDS_engine/engine.h"

#include "gamestatemaingame.h"


//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();

    srand(time(nullptr));

    GameStateMainGame mainGame;

    while(1) {
        mainGame.run();
    }

    return 0;
}
