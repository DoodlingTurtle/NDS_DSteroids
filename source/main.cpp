#include <math.h>
#include <stdlib.h>

#include <nds.h>
#include <time.h>
#include <maxmod9.h>

#include "../modules/RGNDS_Engine/engine.h"

#include "gamestatetitle.h"
#include "gamestatemaingame.h"
#include "gamestategameover.h"
#include "gamestatesetupcontroller.h"
#include "gamestatecredits.h"

#include "asteroid.h"
#include "mm_types.h"
#include "star.h"

#include "config.h"

#include "res/sb1.h"

//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();
    srand(time(nullptr));
    mmInitDefault("nitro:/audio/sb1.bin");
    
// Ressources available to all gamestates
    Star stars[CNT_STARS];
    Asteroid asteroids[MAX_ASTEROIDS];
    int score = 0;
    int gamecontrolls[4];

    gamecontrolls[GAMEINPUT_ACCELERATE] = KEYPAD_UP;
    gamecontrolls[GAMEINPUT_TURNLEFT] = KEYPAD_LEFT;
    gamecontrolls[GAMEINPUT_TURNRIGHT] = KEYPAD_RIGHT;
    gamecontrolls[GAMEINPUT_FIRE] = KEYPAD_R;

// Initialize Asteroids for the very first start
    for(int a = 0; a < 6; a++)
        asteroids[a].bringBackToLife({Engine_RandF() * 256, Engine_RandF() * SCREEN_HEIGHT*2}, true, (1+round(Engine_RandF() * 3)) * 0.25f);

// Define gamestates
    GameStateMainGame mainGame;
        mainGame.asteroids = asteroids;
        mainGame.score = &score;
        mainGame.stars = stars;
        mainGame.game_difficulty = 1;
        mainGame.keys = gamecontrolls;

    GameStateGameOver gameOver;
        gameOver.score = &score;
        gameOver.asteroids = asteroids;
        gameOver.stars = stars;

    GameStateTitle title;
        title.asteroids = asteroids;
        title.stars = stars;

    GameStateSetupController controller;
        controller.keys = gamecontrolls;
      
    GameStateCredits credits;


// Main loop
    while(1) {
        title.run();
        switch(title.selected()) {
            case 0: // New Game
                score = 0;
                mainGame.game_difficulty = 1;

                while(1) {
                    mainGame.run();
                    if(!mainGame.wasGameWon())
                        break;
                    mainGame.game_difficulty += 1;
                }
                gameOver.run();
                break;

            case 1: //Options - Menu
                controller.run();
                break;

            case 2:
                credits.run();
        }
    }

    return 0;
}
