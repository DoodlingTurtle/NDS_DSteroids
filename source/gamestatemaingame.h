#ifndef GAMESTATEMAINGAME_H
#define GAMESTATEMAINGAME_H

#include <cstdio>

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/broadcast.h"
#include "broadcastchannels.h"

#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 48

struct MainGameUpdateData {
    float deltaTime; 
    int keys_held; 
    int keys_up; 
    int keys_justpressed; 
    touchPosition touch;
};

struct MainGameDrawData {
    float deltaTime;
    RGNDS::Engine::Screen screen;
};

class GameStateMainGame : public RGNDS::Engine
{
    public:
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
        Asteroid asteroids[MAX_ASTEROIDS];
        std::vector<Star> stars;

        RGNDS::Transform scorelocation;
        float scoreTimer;

        RGNDS::Broadcast mainGameBroadcast;

    protected:
        int onStart();
        void onEnd();
        void onUpdate(float deltaTime);
        void onDraw(float deltaTime, RGNDS::Engine::Screen screen);

};

#endif // GAMESTATEMAINGAME_H
