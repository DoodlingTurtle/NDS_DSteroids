/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>

#include <nds.h>
#include <gl2d.h>

#include "../modules/RGNDS_engine/engine.h"

#include "./ship.h"


class PixelEngine : public RGNDS::Engine {

private:
    Ship* ship;

protected:

    int onStart() {
        ship = new Ship();
        return 0;
    }

    void onEnd() {
        delete ship;
    }

    void onUpdate(float deltaTime) {
        touchPosition touch;
        touchRead(&touch);
        scanKeys();

        ship->update(deltaTime, keysHeld(), keysUp(), keysDown(), touch);
    }

    void onDraw(float deltaTime, int screen) {

        glBegin2D();
            if(screen == 0) {   // Draw the following only on the top-screen
                char buffer[300];
                sprintf(buffer, "truePos: %f x %f\naccel: %f"
                    , ship->pos.x
                    , ship->pos.y
                    , ship->ph.acceleration
                );
                this->drawText(10, 10, buffer, ARGB16(1, 0, 15, 31));
            }

            ship->draw(screen);
        glEnd2D();
    }

};


//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();

    PixelEngine game;
    game.run();

    return 0;
}
