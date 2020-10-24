#ifndef SHIP_H
#define SHIP_H

#include <nds.h>
#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "./wraparoundrenderer.h"
#include "./broadcastchannels.h"
#include "./broadcast.h"

#include "shipengine.h"
#include "spaceobj.h"

#include "shot.h"

class Ship : public SpaceObj {
    public:
        Broadcast* broadcast;
    
        ShipEngine ph;

        Ship();
        virtual ~Ship();

        void update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch);
        void draw();
        void reset();

    private:
        float angRes = PI/20;
        bool thrusting = false;

        RGNDS::GL2D::PolyShape* shaBody;
        RGNDS::GL2D::PolyShape* shaThruster;

        std::vector<Shot*> *shots;
};

#endif // SHIP_H
