/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>

#include <nds.h>
#include <gl2d.h>
#include <time.h>

#include "../modules/RGNDS_engine/engine.h"

#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 32


class PixelEngine : public RGNDS::Engine {

private:
    Ship ship;
    Asteroid asteroids[MAX_ASTEROIDS];

protected:

    int onStart() {
        int a;

        for(a = 0; a < 6; a++) {
            asteroids[a].bringBackToLife(ship.pos, false, 1);
            asteroids[a].moveInDirection((16 * ship.scale) + 32 + Engine_RandF() * 64);
        }

        return 0;
    }

    void onEnd() {
    }

    void onUpdate(float deltaTime) {
        touchPosition touch;
        touchRead(&touch);
        scanKeys();

        int a;

        ship.update(deltaTime, keysHeld(), keysUp(), keysDown(), touch);

        for(a = 0; a < MAX_ASTEROIDS; a++)
            asteroids[a].update(deltaTime);
    }

    void onDraw(float deltaTime, int screen) {
        int a;
        glBegin2D();

            for(a = 0; a < MAX_ASTEROIDS; a++)
                asteroids[a].draw(screen);

            ship.draw(screen);

            if(screen == 0) {   // Draw the following only on the top-screen
                char buffer[300];
                sprintf(buffer, "truePos: %f x %f\naccel: %f"
                    , ship.pos.x
                    , ship.pos.y
                    , ship.ph.acceleration
                );
                drawText(10, 10, buffer, ARGB16(1, 0, 15, 31));
            }


        glEnd2D();
    }

};


//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();

    srand(time(nullptr));

    PixelEngine game;
    game.run();

    return 0;
}
