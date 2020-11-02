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
}

GameStateMainGame::GameStateMainGame() {

    Shot::shotGameObjects = newGameObjects;

/*
    onAsteroidBroadcast = [this](int event, void* data) {
        Engine_Log("Asteroids broadcast "<<event);
        switch(event) {
        case bceHitPlayer:
            Engine_Log("Asteroid hit Ship");
            exit();     // Meteor hits player = game over;
            break;
        case bceDead:   // In case of bceDead - Event stop heartbeat to the sending asteroid
            Engine_Log("Kill Asteroid");

			for(int a = asteroids.size()-1; a >= 0; a--) {
				if(asteroids.at(a) == ((Asteroid*)data)) {
					Asteroid* ast = (Asteroid*)data;
					ast->kill(&mainGameBroadcast);
					Engine_Log("delete asteroid data/pointer " << ast->scale/2);

					asteroids.erase(asteroids.begin()+a);
					if(ast->scale > 0.25) {
                        Asteroid* ast2;
						for(int b = 0; b < 2; b++) {
							ast2 = new Asteroid();
                            ast2->scale = ast->scale;
                            ast2->pos.x = ast->pos.x;
                            ast2->pos.y = ast->pos.y;
                            asteroidsToRevive.push_back(ast2);
						}
					}

                    int addScore = 100/ast->scale;

                    this->score += addScore;
                    ScorePopup::spawn(addScore, ast->pos.x, ast->pos.y);
					delete ast;
					break;
				}
			}

            //DONE; Award Points
        }
    };
*/
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
    int a;

    Engine_Log("Game Over");
	
    Engine_Log("clean Score Popup");
    ScorePopup::cleanup();

    Engine_Log("clean asteroids;");
    for(a = 0; a < MAX_ASTEROIDS; a++)
        asteroids[a].kill();
	
    Engine_Log("Clean Shots");
    Shot::cleanup();

	Engine_Log("Detach all game components");	
    gameObjects->clear();
    prevGameObjects->clear();
    newGameObjects->clear();

	// Detach all game components from each other
    Engine_Log("Clear stars");
}

void GameStateMainGame::onUpdate(float deltaTime) {
    tick++;
    tick = (tick&1);

    gameObjects         = &gameObjList[0 + tick];
    prevGameObjects     = &gameObjList[1 - tick];

// Add Gameobjects, that are still alive to the cycle
    gameObjects->clear();
    for(SpaceObj* go : *prevGameObjects)
        if(go->isAlive())
            gameObjects->push_back(go);
    
// Add new GameObjects to the cycle
    for(SpaceObj* go : *newGameObjects)
        if(go->isAlive())
            gameObjects->push_back(go);
    
    newGameObjects->clear();

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


}

void GameStateMainGame::onDraw(float deltaTime, RGNDS::Engine::Screen screen) {

// Draw Stars
    for(auto star : stars)
        RGNDS::GL2D::glPixel(star.x, star.y, Engine_Color16(1, 15, 15, 15), star.alpha);

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

