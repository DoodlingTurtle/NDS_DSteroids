#ifndef GAMESTATEMAINGAME_H
#define GAMESTATEMAINGAME_H

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/broadcast.h"

#include "./ship.h"
#include "./asteroid.h"
#include "./star.h"

#include "./gamestatepause.h"

#include "config.h"

class GameStateMainGame : public RGNDS::Engine
{
    public:
		GameStateMainGame();
        virtual ~GameStateMainGame();
        
        int *score;
        Star *stars;
		Asteroid* asteroids;

        float game_difficulty;        // difficulty goes from 1 to 16;
    
        bool wasGameWon();
        
        int* keys;


        void onDraw(RGNDS::Engine::Screen screen);


    private:

        Ship ship;
        AsteroidExplosion asteroidexplosions[MAX_ASTEROIDS];
        
        RGNDS::Transform scorelocation;
        float scoreTimer;

        GameStatePause* pauseState; 

    protected:
        int onStart();
        void onEnd();
        void onUpdate(float deltaTime);

};

#endif // GAMESTATEMAINGAME_H
