#include "gamestatemaingame.h"

#include <climits>

float GameStateMainGame::game_difficulty = 1.0f;

GameStateMainGame::GameStateMainGame() {

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
						Asteroid* ast2 = new Asteroid();
						ast2->bringBackToLife(&mainGameBroadcast, ast->pos, true, ast->scale/2);
						asteroids.push_back(ast2);
					}
					delete ast;
					break;
				}
			}

            //TODO; Award Points
        }
    };
}

int GameStateMainGame::onStart() {
    int a;

    Engine_Log("Start application");
	

    // setup the scoreboard
    scorelocation.pos.x = 5;
    scorelocation.pos.y = 5;
    scorelocation.scale = 2;
    score = 0;
    scoreTimer = 0.0f;
    game_difficulty = 4;

    // Create a star field for the background
    for(a = 0; a < 64; a++) {
        stars.push_back((const Star){
                rand() / (INT_MAX/256),
                rand() / (INT_MAX/(SCREEN_HEIGHT*2)),
                rand() / (INT_MAX/15) + 16
        });
    }

    // Initialize the asteroids
    Engine_Log("setting ub asteroids");
    for(a = 0; a < (int)game_difficulty; a++) {
        Asteroid* ast = new Asteroid();
		ast->bringBackToLife(&mainGameBroadcast, ship.pos, false, 1);
		ast->moveInDirection(64);
		asteroids.push_back(ast);
    }


    // Attach all the game components to each other 
    Asteroid::broadcast.subscribe(&onAsteroidBroadcast);
    mainGameBroadcast.subscribe(&Shot::heartbeat);
    Shot::broadcast.subscribe(&Asteroid::onShotAction);
	ship.broadcast.subscribe(&Asteroid::onShipAction);
    mainGameBroadcast.subscribe(&ship.heartbeat);
	
    ship.reset();
    Engine_Log("Ship transmit Spawn-Event");
    ship.broadcast.transmit(bceSpawn, &ship);

    return 0;
}

void GameStateMainGame::onEnd() {

    Engine_Log("Game Over");

	for(auto a : asteroids) {
		a->kill(&mainGameBroadcast);
		delete a;
	}
	asteroids.clear();

	Engine_Log("Detach all game components");	

	// Detach all game components from each other
    mainGameBroadcast.unsubscribe(&ship.heartbeat);
	ship.broadcast.unsubscribe(&Asteroid::onShipAction);
    Shot::broadcast.unsubscribe(&Asteroid::onShotAction);
    mainGameBroadcast.unsubscribe(&Shot::heartbeat);
    Asteroid::broadcast.unsubscribe(&onAsteroidBroadcast);

    stars.clear();

}

void GameStateMainGame::onUpdate(float deltaTime) {

// Read Player input
    MainGameUpdateData data;

    touchRead(&(data.touch));
    scanKeys();

    data.keys_held = keysHeld();
    data.keys_up   = keysUp();
    data.keys_justpressed = keysDown();
    data.deltaTime = deltaTime;

// Send out an update heartbeat to all attached objects
    mainGameBroadcast.transmit(bceTick, &data);

// Update Scores
    scoreTimer += deltaTime * 1000.0f;
    // award 1 Point for each Second survived
    if(scoreTimer > 1000.0f) {
        score++;
        scoreTimer -= 1000.0f;
    }

    //TODO: punish player if he stayed still for to long
    //TODO: alternatively award points for ship movement instead of time
}

void GameStateMainGame::onDraw(float deltaTime, RGNDS::Engine::Screen screen) {

// Draw Stars
    for(auto star : stars)
        RGNDS::GL2D::glPixel(star.x, star.y, Engine_Color16(1, 15, 15, 15), star.alpha);

// Sendout a draw heartbeat
    MainGameDrawData data = { deltaTime, screen };
    mainGameBroadcast.transmit(bceDraw, &data);    

// Render Score, but only if the Top-Screen is rendered
    if(screen == ENGINE_SCREEN_TOP) {
        char buffer[16];
        sprintf(buffer, "Score: % 8d", score);
        RGNDS::GL2D::glText(buffer, Engine_Color16(1, 0, 10, 31), &scorelocation);
    }
}

