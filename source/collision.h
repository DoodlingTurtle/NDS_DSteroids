#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "../modules/RGNDS_Engine/engine.h"

class Collision
{
    public:
        static bool checkCircleOnCircle(RGNDS::Point<float>* const pos1, float radius1, RGNDS::Point<float>* const pos2, float radius2, Collision* overlapdata=nullptr );


        float C2COverlapDist;
        float C2COverlapImpact;
        RGNDS::Point<float> overlapDir;



        Collision();
        virtual ~Collision();
    protected:

    private:
};

#endif // __COLLISION_H__
