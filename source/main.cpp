/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <climits>

#include <nds.h>
#include <time.h>

#include "../modules/RGNDS_engine/engine.h"

#include "./ship.h"
#include "./asteroid.h"

#define MAX_ASTEROIDS 48


class PixelEngine : public RGNDS::Engine, public Broadcast::Listener {

private:

    struct Star {
        int x;
        int y;
        int alpha;
    };


    Ship ship;
    Asteroid asteroids[MAX_ASTEROIDS];
    std::vector<Star> stars;

    RGNDS::Transform scorelocation;
    float scoreTimer;

    void onBroadcast(int channel, int event, void* sender) {
        if(channel == bchAsteroid) {
            switch(event) {
            case bceHitPlayer:
                //exit();     // Meteor hits player = game over;
                break;
            case bceDead:
                ((Asteroid*)sender)->kill();
                ship.broadcast->unsubscribe((Asteroid*)sender);
                //TODO; Award Points
            }
        }
    }

protected:

    static float game_difficulty;        // difficulty goes from 1 to 16;
                                         // 16 = max num of Asteroids with biggest size


    int onStart() {
        int a;

        Asteroid::broadcast.subscribe(this);
        scorelocation.pos.x = 5;
        scorelocation.pos.y = 5;
        scorelocation.scale = 2;

        score = 0;
        scoreTimer = 0.0f;
        game_difficulty = 4;

        for(a = 0; a < 64; a++) {
            stars.push_back((const Star){
                    rand() / (INT_MAX/256),
                    rand() / (INT_MAX/(SCREEN_HEIGHT*2)),
                    rand() / (INT_MAX/15) + 16
            });
        }

        for(a = 0; a < (int)game_difficulty; a++) {
            asteroids[a].bringBackToLife(ship.pos, false, 1);
            asteroids[a].tra->moveInDirection((16 * ship.scale) + 32 + Engine_RandF() * 64);
            ship.broadcast->subscribe(&asteroids[a]);
        }

        return 0;
    }

    void onEnd() {
        for(int a = 0; a < MAX_ASTEROIDS; a++) {
            ship.broadcast->unsubscribe(&asteroids[a]);
            asteroids[a].kill();
        }

        Asteroid::broadcast.unsubscribe(this);
        stars.clear();
    }

    void onUpdate(float deltaTime) {
        touchPosition touch;
        touchRead(&touch);
        scanKeys();

        scoreTimer += deltaTime * 1000.0f;
        // award 1 Point for each Second survived
        if(scoreTimer > 1000.0f) {
            score++;
            scoreTimer -= 1000.0f;
        }

        //TODO: punish plazer if he stayed still for to long
        //TODO: alternatively award points for ship movement instead of time

        int a;

        for(a = 0; a < MAX_ASTEROIDS; a++)
            asteroids[a].update(deltaTime);

        ship.update(deltaTime, keysHeld(), keysUp(), keysDown(), touch);
    }

    void onDraw(float deltaTime, int screen) {
        int a;

        for(auto star : stars) {
            RGNDS::GL2D::glPixel(star.x, star.y, Engine_Color16(1, 15, 15, 15), star.alpha);
        }

        for(a = 0; a < MAX_ASTEROIDS; a++)
            asteroids[a].draw(screen);

        ship.draw(screen);

        if(screen == 0) {
            char buffer[16];
            sprintf(buffer, "Score: % 8d", score);
            RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &scorelocation);
        }
    }

public:
    int score = 0;

};

float PixelEngine::game_difficulty = 1.0f;


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
