#include <climits>
#include <cstdio>

#include <nds.h>

#include "gamestatemaingame.h"
#include "gamestatepause.h"
#include "scorepopup.h"

#include "res/sb1.h"
#include "shipstats.h"

static std::vector<SpaceObj*>   gameObjList[3];
static std::vector<SpaceObj*>*  gameObjects         = &gameObjList[0];
static std::vector<SpaceObj*>*  prevGameObjects     = &gameObjList[1];
static std::vector<SpaceObj*>*  newGameObjects      = &gameObjList[2];
static byte tick = 0;

static ShipExplosion* shipexp = nullptr;

GameStateMainGame::~GameStateMainGame() {
    Shot::shotGameObjects = nullptr;
}

GameStateMainGame::GameStateMainGame(ShipStats *shipstats) {
    Shot::shotGameObjects = newGameObjects;
    ship.stats = shipstats;
}

int GameStateMainGame::onStart() {
    int a;
    
    ship.controlls = keys;

    Asteroid::init(&ship);

    Engine_Log("Start MainGame");

    // setup the scoreboard
    scorelocation.pos.x = 5;
    scorelocation.pos.y = 5;
    scorelocation.scale = 2;
    scoreTimer = 0.0f;

    Engine_Log("Reset Ship");
    ship.reset();
    newGameObjects->push_back(&ship);

    // Initialize the asteroids
    for(int a = 0; a < MAX_ASTEROIDS; a++)
        asteroids[a].kill();

    for(a = 0; a < (int)game_difficulty; a++) {
        Asteroid* ast = &asteroids[a];
        ast->bringBackToLife(ship.pos, true, 1);
        ast->moveInDirection(64);

        newGameObjects->push_back(ast);
    }

    

    return 0;
}

void GameStateMainGame::onEnd() {

    if(shipexp != nullptr)
        delete shipexp;
    
    shipexp = nullptr;

    ScorePopup::cleanup();
	
    Shot::cleanup();

    gameObjects->clear();
    prevGameObjects->clear();
    newGameObjects->clear();

    Asteroid::deinit();
}

void GameStateMainGame::onUpdate(float deltaTime) {

// Read Player input
    SpaceObj::MainGameUpdateData data;

    touchRead(&(data.touch));
    scanKeys();

    data.keys_held = keysHeld();    
    data.keys_up   = keysUp();
    data.keys_justpressed = keysDown();
    data.deltaTime = deltaTime;

// Check if the pause key (start has been pressed)
    if((data.keys_justpressed&(KEY_START|KEY_LID)) != 0) {
        GameStatePause* p = new GameStatePause(this);
        p->run();
        bool b = p->endGame();
        delete p;

        if(b) {
            // Kill the Ship, to signalize a GameOver
            ship.kill();
            // End the game
            exit();
        }

        return;
    }

// switch gameobject lists
    tick++;
    tick = (tick&1);

    gameObjects         = &gameObjList[0 + tick];
    prevGameObjects     = &gameObjList[1 - tick];

// Add Gameobjects, that are still alive to the cycle
    gameObjects->clear();
    bool asteroidFound = false;
    bool hasAsteroids = false;
    for(SpaceObj* go : *prevGameObjects) {
        asteroidFound = go >= asteroids && go < (&asteroids[MAX_ASTEROIDS-1]);
        hasAsteroids |= asteroidFound;

        if(go->isAlive()) {
            gameObjects->push_back(go);
        }
        else {
            Engine_Log("ignoring dead GameObject " << go);
            short addScore = go->getScoreValue();
            if(addScore > 0) {
                *score += addScore;    
            }

            // Check if killed object is part of the Asteroids List
            if(asteroidFound) {
                // If yes, Spawn ScorePopups
                newGameObjects->push_back(ScorePopup::spawn(addScore, go->pos.x, go->pos.y));
					
                // And if scale is bigger then 25% spawn 2 new Asteroids at half the size of the killed one
                if(go->scale > 0.25) {
                    byte found = 0;
                    for(int a = 0; a < MAX_ASTEROIDS; a++) {
                        if(found > 1) break;
                        if((asteroids[a]).isAlive()) continue;
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

                for(int a = 0; a < MAX_ASTEROIDS; a++) {
                    if(asteroidexplosions[a].isAlive()) continue;
                    asteroidexplosions[a].revive(go->pos.x, go->pos.y, 4.0f * go->scale);
                    newGameObjects->push_back(&asteroidexplosions[a]);
                    break;
                }
            }
        }
    }

// Add new GameObjects to the cycle
    for(SpaceObj* go : *newGameObjects)
        if(go->isAlive()) {
            Engine_Log("adding GameObject " << go);
            gameObjects->push_back(go);
            hasAsteroids |= (go >= asteroids && go < (&asteroids[MAX_ASTEROIDS-1]));
        }
    
    newGameObjects->clear();

    if(!hasAsteroids) {
        this->exit();
        return;
    }

// Update all GameObject-Managers/Factorys
    ScorePopup::refreshInstanceList();

// Send out an update heartbeat to all attached objects
    for(SpaceObj* go : *gameObjects) {
        go->onUpdate(&data);
    }

// If Ship is dead, exit game
// TODO: (DoTu) add "multiple lifes game mechanic"
    if(!ship.isAlive() and shipexp == nullptr) {
        Asteroid::ship = nullptr;

        shipexp = new ShipExplosion(&ship);
        newGameObjects->push_back(shipexp);
    }
    else if(shipexp != nullptr and !shipexp->isAlive()) {
        exit();
    }

}

void GameStateMainGame::onDraw(RGNDS::Engine::Screen screen) {

// Draw Stars
    for(int a = 0; a < CNT_STARS; a++)
        stars[a].draw();
        
// Sendout a draw heartbeat
    SpaceObj::MainGameDrawData data = { 0.0f , screen };

    for(SpaceObj* go : *gameObjects)
        go->onDraw(&data);

// Render Score, but only if the Top-Screen is rendered
    if(screen == ENGINE_SCREEN_TOP) {
        char buffer[16];
        sprintf(buffer, "Score: % 8d", *score);
        RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &scorelocation);
    }
}

bool GameStateMainGame::wasGameWon() {
    return ship.isAlive();
}
