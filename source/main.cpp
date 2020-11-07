/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>

#include <nds.h>
#include <time.h>

#include "../modules/RGNDS_Engine/engine.h"

#include "gamestatemaingame.h"
#include "gamestategameover.h"

#include "asteroid.h"
#include "star.h"

#include "config.h"

//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();

    srand(time(nullptr));

    Star stars[CNT_STARS];
    Asteroid asteroids[MAX_ASTEROIDS];
    int score = 0;

    GameStateMainGame mainGame;
        mainGame.asteroids = asteroids;
        mainGame.score = &score;
        mainGame.stars = stars;

    GameStateGameOver gameOver;
        gameOver.score = &score;
        gameOver.asteroids = asteroids;
        gameOver.stars = stars;

    while(1) {
        gameOver.run();
        mainGame.run();
    }

    return 0;
}
