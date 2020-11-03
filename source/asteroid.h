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
        static Ship* ship;
        
        Asteroid();
        virtual ~Asteroid();

        void update(float deltaTime);
       
       static std::function<void(int, void*)> onShipAction;

        void bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale);

        void onDraw(SpaceObj::MainGameDrawData*);
        void onUpdate(SpaceObj::MainGameUpdateData*);

        short getScoreValue();

    protected:
        // Objects for Asteroid to keep track of
        static std::vector<Shot*> shots;

    private:
        void generateShape();
        float spinSpeed;
};

#endif
