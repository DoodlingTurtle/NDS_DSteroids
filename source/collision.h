#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "../modules/RGNDS_Engine/engine.h"

namespace RGNDS {
    class Collision
    {
        public:


            /** \brief tells if a collision between 2 Circles has happened
             *  \param pos1 center of the first circle
             *  \param radius1 radius of the first circle
             *  \param pos2 center of the second circle to
             *  \param radius2 radius of the second circle are close enough to overlap
             *
             *  \param overlapdata (default: nullptr) if defined, this gets filled out with more details about the collision
             *                                        - C2COverlapDist this distance, that both circles must have, to not collide
             *                                        - C2COverlapImpact the distance from pos1 to pos2, wenn both shapes collide
             *                                        - overlapDir - the vector of the direction, from circle1 to circle2
             *
             *  \return - true if both circles are
             */
            static bool checkCircleOnCircle(Point<float>* const pos1, float radius1, Point<float>* const pos2, float radius2, Collision* overlapdata=nullptr );




       // Propertys have different meaning depenging, on which function filled them in
            float C2COverlapDist;
            float C2COverlapImpact;
            Point<float> overlapDir;

            Collision();
            virtual ~Collision();
        protected:

        private:
    };
}
#endif // __COLLISION_H__
