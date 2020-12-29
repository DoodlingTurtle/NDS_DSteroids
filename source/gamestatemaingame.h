#ifndef GAMESTATEMAINGAME_H
#define GAMESTATEMAINGAME_H

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/broadcast.h"

#include "./ship.h"
#include "./asteroid.h"
#include "./star.h"

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

    private:

        Ship ship;
        AsteroidExplosion asteroidexplosions[MAX_ASTEROIDS];
        
        RGNDS::Transform scorelocation;
        float scoreTimer;

    protected:
        int onStart();
        void onEnd();
        void onUpdate(float deltaTime);
        void onDraw(RGNDS::Engine::Screen screen);

};

#endif // GAMESTATEMAINGAME_H
