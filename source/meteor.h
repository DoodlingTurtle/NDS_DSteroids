#ifndef METEOR_H
#define METEOR_H

#include "../modules/RGNDS_Engine/engine.h"
#include "wraparoundrenderer.h"

class Meteor : public RGNDS::Engine::PolyObj
{
    public:
        Meteor();
        virtual ~Meteor();

        void update(float deltaTime);

        void draw(int screen);

    private:
        WrapAroundRenderer renderer;
        RGNDS::Point<float> velocity;
};

#endif // METEOR_H
