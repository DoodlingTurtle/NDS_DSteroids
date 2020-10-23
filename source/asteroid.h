#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "wraparoundrenderer.h"

#include "../modules/RGNDS_Engine/transform.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"

#include "broadcast.h"
#include "broadcastchannels.h"

class Asteroid : public RGNDS::GL2D::PolyShape, public Broadcast::Listener
{
    public:

        static Broadcast broadcast;

        RGNDS::Transform* tra = nullptr;

        Asteroid();
        virtual ~Asteroid();

        void update(float deltaTime);

        void draw();

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
