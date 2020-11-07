#include "star.h"
#include "../modules/RGNDS_Engine/gl2d.h"
#include <climits>
#include "../modules/RGNDS_Engine/engine.h"

Star::Star() {
    x = rand() / (INT_MAX/256);
    y = rand() / (INT_MAX/(SCREEN_HEIGHT*2));
    alpha = rand() / (INT_MAX/15) + 16;
}

void Star::draw(){
    RGNDS::GL2D::glPixel(x, y, Engine_Color16(1, 15, 15, 15), alpha, -1);
}
