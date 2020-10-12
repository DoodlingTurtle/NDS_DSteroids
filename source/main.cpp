/*-----------------------------------------------------------------------------

	Basic template code for starting a DS app

-----------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>

#include <nds.h>
#include <gl2d.h>

#include "./res/bounceBall32x64.h"
#include "./RGNDS/engine.h"

#define SCREEN_HEIGHT2 384

typedef RGNDS::Point<float> PointF;


class Ship : public RGNDS::Engine::PolyObj
{
private:
    float angRes = PI/20;
    bool thrusting = false;

    PolyObj* thruster;

    PointF drawingInstances[4];
    byte drawingInstanceCnt = 1;

public:
    PointF truePosition;


    Ship() : PolyObj(2, (const PointF[6]){
        {  8.0f,  0.0f }
      , { -4.0f,  0.0f }
      , { -8.0f, -8.0f }

      , {  8.0f,  0.0f }
      , { -4.0f,  0.0f }
      , { -8.0f,  8.0f }
    }){

    // Initialize the Trhuster (yellow triangle at the bottom of the ship);
        thruster = new RGNDS::Engine::PolyObj(1, (const PointF[3]){
            { -8.0f,  0.0f }
          , { -4.0f, -4.0f }
          , { -4.0f,  4.0f }
        });

    // Setup Coordinats of the ship and varables needed for Wrap-Arround functionality
        pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        truePosition = pos;
        drawingInstances[0] = {0, 0};       // Drawing Instance will always be 0 (aka the true position of the Object)



    }

    ~Ship() {
        delete thruster;
    }

    void update(float deltaTime, int keys_held, int keys_up, int keys_justpressed, touchPosition& touch) {

    // Process user Input
        if(keys_held&KEY_RIGHT)
            setAngleRel(angRes);

        if(keys_held&KEY_LEFT)
            setAngleRel(-angRes);

        if(keys_held&KEY_X)
            scale += 0.05;

        if(keys_held&KEY_B)
            scale -= 0.05;

        if(keys_held&KEY_UP) {
            moveInDirection(64 * scale * deltaTime);
            thrusting = true;
        }
        else {
            thrusting = false;
        }


    // Check for WrapAround
        float shipRadius = 8 * scale;
        //TODO: (RGO) Make sure ship radius does not eclipse SCREEN_HEIGHT (192px)


        // Move ship back to screen, once its true Position has left the Screen completely
        if(pos.x >= SCREEN_WIDTH + shipRadius)
            pos.x -= SCREEN_WIDTH;

        if(pos.x <= -shipRadius)
            pos.x += SCREEN_WIDTH;

        if(pos.y >= SCREEN_HEIGHT2 + shipRadius)
            pos.y -= SCREEN_HEIGHT2;

        if(pos.y <= -shipRadius)
            pos.y += SCREEN_HEIGHT2;

        // Reset all the Position Data
        this->truePosition = this->pos;
        drawingInstances[0] = pos;
        drawingInstanceCnt = 1;

        // Left Screen border wrap arround
        if(pos.x > SCREEN_WIDTH-shipRadius) {
            drawingInstances[drawingInstanceCnt] = { pos.x - SCREEN_WIDTH, pos.y };
            drawingInstanceCnt++;
        }

        // Right Screen border
        else if(pos.x < shipRadius) {
            drawingInstances[drawingInstanceCnt] = { pos.x + SCREEN_WIDTH, pos.y };
            drawingInstanceCnt++;
        }

        // Bottom Screen border
        if(pos.y > SCREEN_HEIGHT2-shipRadius) {
            drawingInstances[drawingInstanceCnt] = { pos.x, pos.y - SCREEN_HEIGHT2 };
            drawingInstanceCnt++;
        }

        // Top Screen border
        else if(pos.y < shipRadius) {
            drawingInstances[drawingInstanceCnt] = { pos.x, pos.y + SCREEN_HEIGHT2 };
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

    void draw(int screen) {


        for(int a = 0; a < drawingInstanceCnt; a++) {
            pos = drawingInstances[a];

            if(thrusting)
                thruster->draw(0x83ff, screen, this);

            RGNDS::Engine::PolyObj::draw(0x801F, screen);
        }

        pos = truePosition;

    }



};


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
                sprintf(buffer, "truePos: %f x. %f"
                    , ship->truePosition.x
                    , ship->truePosition.y
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
