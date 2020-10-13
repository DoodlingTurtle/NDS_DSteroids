#include "wraparoundrenderer.h"

WrapAroundRenderer::WrapAroundRenderer() {
    drawingInstances[0] = {0, 0};
}

WrapAroundRenderer::~WrapAroundRenderer() {}

void WrapAroundRenderer::updateDrawingInstances(RGNDS::Point<float>* pos, float shipRadius) {


    // Move ship back to screen, once its true Position has left the Screen completely
    if(pos->x >= right + shipRadius)
        pos->x -= width;

    if(pos->x <= left-shipRadius)
        pos->x += width;

    if(pos->y >= bottom + shipRadius)
        pos->y -= height;

    if(pos->y <= top-shipRadius)
        pos->y += height;


    drawingInstances[0] = { pos->x, pos->y };
    drawingInstanceCnt = 1;

    // Left Screen border wrap arround
    if(pos->x > right-shipRadius) {
        drawingInstances[drawingInstanceCnt] = { pos->x - width, pos->y };
        drawingInstanceCnt++;
    }

    // Right Screen border
    else if(pos->x < left + shipRadius) {
        drawingInstances[drawingInstanceCnt] = { pos->x + width, pos->y };
        drawingInstanceCnt++;
    }

    // Bottom Screen border
    if(pos->y > bottom-shipRadius) {
        drawingInstances[drawingInstanceCnt] = { pos->x, pos->y - height };
        drawingInstanceCnt++;
    }

    // Top Screen border
    else if(pos->y < top + shipRadius) {
        drawingInstances[drawingInstanceCnt] = { pos->x, pos->y + height };
        drawingInstanceCnt++;
    }

    // Opposit Cornor wrap around (if there is a wrap around on both x and y axis, draw a
    // fourth ship on the opposit cornor of the screen, from where the true ship is
    if(drawingInstanceCnt == 3) {
        drawingInstances[drawingInstanceCnt] = {
            drawingInstances[1].x
          , drawingInstances[2].y
        };
        drawingInstanceCnt++;
    }
}

void WrapAroundRenderer::defineWrappingArea(int t, int r, int b, int l) {
    top = t;
    right = r;
    left = l;
    bottom = b;
    width = r - l;
    height = b - t;
}

byte WrapAroundRenderer::getInstanceCnt() {
    return drawingInstanceCnt;
}

RGNDS::Point<float> WrapAroundRenderer::getInstance(byte index) {
    if(index < 0 || index > 3)
        index = 0;

    return drawingInstances[index];
}
