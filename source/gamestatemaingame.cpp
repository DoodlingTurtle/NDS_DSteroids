#include <nds.h>
#include "gamestatemaingame.h"
#include "scorepopup.h"

#include <climits>
#include <cstdio>

float GameStateMainGame::game_difficulty = 1.0f;

static std::vector<SpaceObj*>   gameObjList[3];
static std::vector<SpaceObj*>*  gameObjects         = &gameObjList[0];
static std::vector<SpaceObj*>*  prevGameObjects     = &gameObjects[1];
static std::vector<SpaceObj*>*  newGameObjects      = &gameObjects[2];
static byte tick = 0;

GameStateMainGame::~GameStateMainGame() {
    Shot::shotGameObjects = nullptr;
    Asteroid::ship = nullptr;
}

GameStateMainGame::GameStateMainGame() {
    Asteroid::ship = &ship;
    Shot::shotGameObjects = newGameObjects;
}

int GameStateMainGame::onStart() {
    int a;

    Engine_Log("Start application");

    Engine_Log("Generate Star field");
    // setup the scoreboard
    scorelocation.pos.x = 5;
    scorelocation.pos.y = 5;
    scorelocation.scale = 2;
    score = 0;
    scoreTimer = 0.0f;
    game_difficulty = 4;

    // Create a star field for the background
    for(a = 0; a < 64; a++) {
        stars[a] = { 
            rand() / (INT_MAX/256),
            rand() / (INT_MAX/(SCREEN_HEIGHT*2)),
            rand() / (INT_MAX/15) + 16
        };
    }

    Engine_Log("Register Ship");
    ship.reset();
    newGameObjects->push_back(&ship);

    // Initialize the asteroids
    Engine_Log("setting ub asteroids");
    for(a = 0; a < (int)game_difficulty; a++) {
        Asteroid* ast = &asteroids[a];
        ast->bringBackToLife(ship.pos, true, 1);
        ast->moveInDirection(64);

        newGameObjects->push_back(ast);
    }
    return 0;
}

void GameStateMainGame::onEnd() {
    Engine_Log("Game Over");

    Engine_Log("clean asteroids;");
    for(int a = 0; a < MAX_ASTEROIDS; a++)
        asteroids[a].kill();

    Engine_Log("clear ScorePopup");
    ScorePopup::cleanup();
	
    Engine_Log("Clean Shot");
    Shot::cleanup();

	Engine_Log("Detach all game components");	
    gameObjects->clear();
    prevGameObjects->clear();
    newGameObjects->clear();
}

void GameStateMainGame::onUpdate(float deltaTime) {
    tick++;
    tick = (tick&1);

    gameObjects         = &gameObjList[0 + tick];
    prevGameObjects     = &gameObjList[1 - tick];

// Add Gameobjects, that are still alive to the cycle
    gameObjects->clear();
    for(SpaceObj* go : *prevGameObjects)
        if(go->isAlive()) {
            gameObjects->push_back(go);
        }
        else {
            short addScore = go->getScoreValue();
            if(addScore > 0) {
                this->score += addScore;    
            }

            // Check if killed object is part of the Asteroids List
            if(go >= asteroids && go < (&asteroids[MAX_ASTEROIDS-1] + sizeof(Asteroid))) {
                // If yes, Spawn ScorePopups
                newGameObjects->push_back(ScorePopup::spawn(addScore, go->pos.x, go->pos.y));
					
                // And if scale is bigger then 25% spawn 2 new Asteroids at half the size of the killed one
                if(go->scale > 0.25) {
                    byte found = 0;
                    for(int a = 0; a < MAX_ASTEROIDS; a++) {
                        if(found > 1) break;
                        if(asteroids[a].isAlive()) continue;
                        if(&asteroids[a] == go) continue;

                        Asteroid* ast2 = &asteroids[a];
                        ast2->bringBackToLife(go->pos, true, go->scale/2);
                        newGameObjects->push_back(ast2);

                        found++;
                    }                        

                    if(found != 2) {
                        Engine_Log("Asteroid ObjectLimit of " << MAX_ASTEROIDS << " reached");
                        found=0;
                    }
                }
            }
        }

// Add new GameObjects to the cycle
    for(SpaceObj* go : *newGameObjects)
        if(go->isAlive())
            gameObjects->push_back(go);
    
    newGameObjects->clear();

// Update all GameObject-Managers/Factorys
    ScorePopup::refreshInstanceList();

// Read Player input
    SpaceObj::MainGameUpdateData data;

    touchRead(&(data.touch));
    scanKeys();

    data.keys_held = keysHeld();    
    data.keys_up   = keysUp();
    data.keys_justpressed = keysDown();
    data.deltaTime = deltaTime;

// Send out an update heartbeat to all attached objects
    for(SpaceObj* go : *gameObjects) {
        go->onUpdate(&data);
    }

// If Ship is dead, exit game
// TODO: (DoTu) add "multiple lifes game mechanic"
    if(!ship.isAlive()) exit();

}

void GameStateMainGame::onDraw(float deltaTime, RGNDS::Engine::Screen screen) {

// Draw Stars
    for(auto star : stars)
        RGNDS::GL2D::glPixel(star.x, star.y, Engine_Color16(1, 15, 15, 15), star.alpha, -1);

// Sendout a draw heartbeat
    SpaceObj::MainGameDrawData data = { deltaTime, screen };

    for(SpaceObj* go : *gameObjects)
        go->onDraw(&data);

// Render Score, but only if the Top-Screen is rendered
    if(screen == ENGINE_SCREEN_TOP) {
        char buffer[16];
        sprintf(buffer, "Score: % 8d", score);
        RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &scorelocation);
    }
}
