#ifndef GAMESTATEMAINGAME_H
#define GAMESTATEMAINGAME_H

#include "../modules/RGNDS_Engine/engine.h"
#include "broadcast.h"
#include "broadcastchannels.h"


#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 48

class GameStateMainGame : public RGNDS::Engine, public Broadcast::Listener
{
    public:
        int score = 0;

        void onBroadcast(int channel, int event, void* sender);

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

    protected:
        int onStart();
        void onEnd();
        void onUpdate(float deltaTime);
        void onDraw(float deltaTime, int screen);

};

#endif // GAMESTATEMAINGAME_H
