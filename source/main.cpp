/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>

#include <nds.h>
#include <time.h>

#include "../modules/RGNDS_engine/engine.h"

#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 32


class PixelEngine : public RGNDS::Engine, public Broadcast::Listener {

private:
    Ship ship;
    Asteroid asteroids[MAX_ASTEROIDS];

    void onBroadcast(int channel, int event, void* sender) {
        if(channel == bchAsteroid) {
            switch(event) {
            case bceHitPlayer:
                exit();
                break;
            case bceDead:
                ((Asteroid*)sender)->kill();
                ship.broadcast->unsubscribe((Asteroid*)sender);
                //TODO; Award Points
            }
        }
    }

protected:

    int onStart() {
        int a;

        Asteroid::broadcast.subscribe(this);

        for(a = 0; a < 6; a++) {
            asteroids[a].bringBackToLife(ship.pos, false, 1);
            asteroids[a].tra->moveInDirection((16 * ship.scale) + 32 + Engine_RandF() * 64);
            ship.broadcast->subscribe(&asteroids[a]);
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

        for(a = 0; a < MAX_ASTEROIDS; a++)
            asteroids[a].update(deltaTime);

        ship.update(deltaTime, keysHeld(), keysUp(), keysDown(), touch);
    }

    void onDraw(float deltaTime, int screen) {
        int a;

        for(a = 0; a < MAX_ASTEROIDS; a++)
            asteroids[a].draw(screen);

        ship.draw(screen);

    }

};


//-----------------------------------------------------------------------------
int main(void) {
//-----------------------------------------------------------------------------
    RGNDS::Engine::init();

    srand(time(nullptr));

    PixelEngine* game = nullptr;
    while(1) {
        game = new PixelEngine();
        game->run();
        delete game;
    }

    return 0;
}
