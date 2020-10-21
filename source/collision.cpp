#include "collision.h"

#include <math.h>

namespace RGNDS {

    bool Collision::checkCircleOnCircle(Point<float>* const pos1, float radius1, Point<float>* const pos2, float radius2, Collision* overlapdata)
    {
        RGNDS::Point<float> d =  *pos1 - *pos2;

        float dstP = (d.x * d.x) + (d.y * d.y);

        float dstR = radius1 + radius2;
              dstR *= dstR;

        if(dstP < dstR){
            if(overlapdata != nullptr) {
                overlapdata->C2COverlapDist = std::sqrt(dstR);
                overlapdata->C2COverlapImpact = std::sqrt(dstR-dstP);
                overlapdata->overlapDir.x = d.x / overlapdata->C2COverlapDist;
                overlapdata->overlapDir.y = d.y / overlapdata->C2COverlapDist;
            }
            return true;
        }

        return false;
    }


    Collision::Collision() {}
    Collision::~Collision() {}
}
