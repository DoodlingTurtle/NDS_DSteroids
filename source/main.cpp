/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdlib.h>

#include <nds.h>
#include <time.h>

#include "../modules/RGNDS_Engine/engine.h"

#include "gamestatetitle.h"
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

// Ressources available to all gamestates
    Star stars[CNT_STARS];
    Asteroid asteroids[MAX_ASTEROIDS];
    int score = 0;

// Initialize Asteroids for the very first start
    for(int a = 0; a < 6; a++)
        asteroids[a].bringBackToLife({Engine_RandF() * 256, Engine_RandF() * SCREEN_HEIGHT*2}, true, (1+round(Engine_RandF() * 3)) * 0.25f);

// Define posible gamestates
    GameStateMainGame mainGame;
        mainGame.asteroids = asteroids;
        mainGame.score = &score;
        mainGame.stars = stars;

    GameStateGameOver gameOver;
        gameOver.score = &score;
        gameOver.asteroids = asteroids;
        gameOver.stars = stars;

    GameStateTitle title;
        title.asteroids = asteroids;
        title.stars = stars;

    while(1) {
        title.run();
        switch(title.selected()) {
            case 0:
                mainGame.run();
                gameOver.run();
        }
    }

    return 0;
}
