#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "../modules/RGNDS_Engine/gl2d_polyshape.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "spaceobj.h"
#include "ship.h"
#include "shot.h"

#include "../modules/RGNDS_Engine/broadcast.h"
#include "broadcastchannels.h"

class Asteroid 
    : public RGNDS::GL2D::PolyShape
    , public SpaceObj
{
    public:
   
        static RGNDS::Broadcast broadcast;
        
        Asteroid();
        virtual ~Asteroid();

        void update(float deltaTime);
       
        std::function<void(int, void*)> onGameHeartbeat;

        static std::function<void(int, void*)> onShotAction;
		static std::function<void(int, void*)> onShipAction;

        void bringBackToLife(RGNDS::Broadcast* gameHeartbeat, RGNDS::Point<float> pos, bool generateNewShape, float scale);
        void kill(RGNDS::Broadcast* gameHeartbeat);

    protected:
        // Objects for Asteroid to keep track of
        static Ship* ship;
        static std::vector<Shot*> shots;

    private:
        void generateShape();
        float spinSpeed;
};

#endif
