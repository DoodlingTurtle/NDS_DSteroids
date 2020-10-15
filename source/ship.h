#ifndef SHIP_H
#define SHIP_H

#include <nds.h>
#include "../modules/RGNDS_Engine/engine.h"

#include "./shipengine.h"
#include "./wraparoundrenderer.h"
#include "./broadcastchannels.h"
#include "./broadcast.h"

#define PointF RGNDS::Point<float>

class Ship : public RGNDS::Engine::PolyObj {
    public:

        PointF velocity;
        ShipEngine ph;
        PointF truePosition;
        Broadcast* broadcast;
        WrapAroundRenderer renderer;

        Ship();
        virtual ~Ship();

        void update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch);
        void draw(int screen);

    private:
        float angRes = PI/20;
        bool thrusting = false;

        RGNDS::Engine::PolyObj* thruster;

};

#endif // SHIP_H
