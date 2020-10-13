#ifndef METEOR_H
#define METEOR_H

#include "../modules/RGNDS_Engine/engine.h"

class Meteor : public RGNDS::Engine::PolyObj
{
    public:
        Meteor();
        virtual ~Meteor();

        void update(float deltaTime);

    private:
};

#endif // METEOR_H
