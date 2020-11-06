#ifndef GAMESTATEMAINGAME_H
#define GAMESTATEMAINGAME_H

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/broadcast.h"

#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 48


class GameStateMainGame : public RGNDS::Engine
{
    public:
		GameStateMainGame();
        virtual ~GameStateMainGame();
        
        int score = 0;

        std::function<void(int, void*)> onAsteroidBroadcast;

    private:
        static float game_difficulty;        // difficulty goes from 1 to 16;

        struct Star {
            int x;
            int y;
            int alpha;
        };

        Ship ship;
        Star stars[64];
		Asteroid asteroids[MAX_ASTEROIDS];
        


        RGNDS::Transform scorelocation;
        float scoreTimer;

    protected:
        int onStart();
        void onEnd();
        void onUpdate(float deltaTime);
        void onDraw(float deltaTime, RGNDS::Engine::Screen screen);

};

#endif // GAMESTATEMAINGAME_H
