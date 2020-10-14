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

        void bringBackToLife(RGNDS::Point<float> pos, bool generateNewShape, float scale);

    private:
        void generateShape();

        WrapAroundRenderer renderer;
        RGNDS::Point<float> velocity;
        float spinSpeed;

        bool alive;
};

#endif // METEOR_H
