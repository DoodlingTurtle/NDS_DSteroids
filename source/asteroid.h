#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "wraparoundrenderer.h"

#include "broadcast.h"
#include "broadcastchannels.h"

class Asteroid : public RGNDS::Engine::PolyObj, public Broadcast::Listener
{
    public:

        static Broadcast broadcast;

        Asteroid();
        virtual ~Asteroid();

        void update(float deltaTime);

        void draw(int screen);

        void bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale);
        void kill();

        void onBroadcast(int c, int e, void* b);

    private:
        void generateShape();

        WrapAroundRenderer renderer;
        RGNDS::Point<float> velocity;
        float spinSpeed;

        bool alive;
};

#endif
