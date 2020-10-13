#ifndef WRAPAROUNDRENDERER_H
#define WRAPAROUNDRENDERER_H

#include "../modules/RGNDS_Engine/engine.h"

class WrapAroundRenderer
{
    public:
        WrapAroundRenderer();
        virtual ~WrapAroundRenderer();

        void updateDrawingInstances(RGNDS::Point<float>* pos, float detectSphere);

        void defineWrappingArea(int top, int right, int bottom, int left);

        byte getInstanceCnt();
        RGNDS::Point<float> getInstance(byte index);

    private:
        int top, right, bottom, left, width, height;

        RGNDS::Point<float> drawingInstances[4];
        byte drawingInstanceCnt = 1;
};

#endif // WRAPAROUNDRENDERER_H
