#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "wraparoundrenderer.h"

class Asteroid : public RGNDS::Engine::PolyObj
{
    public:
        Asteroid();
        virtual ~Asteroid();

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

#endif
