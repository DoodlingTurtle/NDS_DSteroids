#include "gamestatemaingame.h"

#include <climits>

float GameStateMainGame::game_difficulty = 1.0f;


int GameStateMainGame::onStart() {
    int a;

    Engine_Log("Start application");

    // Listen to what all asteroids are doing
    onAsteroidBroadcast = [this](int event, void* data) {
        Engine_Log("Asteroids broadcast "<<event);
        switch(event) {
        case bceHitPlayer:
            Engine_Log("Asteroid hit Ship");
            exit();     // Meteor hits player = game over;
            break;
        case bceDead:   // In case of bceDead - Event stop heartbeat to the sending asteroid
            Engine_Log("Kill Asteroid");
            ((Asteroid*)data)->kill(&mainGameBroadcast);
            //TODO; Award Points
        }
    };
    
    Engine_Log("Engine subscribe to broadcast");
    Asteroid::broadcast.subscribe(&onAsteroidBroadcast);
    
    Engine_Log("Shot subscribe to Heartbeat");
    mainGameBroadcast.subscribe(&Shot::heartbeat);

    Engine_Log("Asteroid Subscribe to Shot Action");
    Shot::broadcast.subscribe(&Asteroid::onShotAction);

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
        // resurect the, until now, dead astroid
        asteroids[a].bringBackToLife(&mainGameBroadcast, ship.pos, false, 1);

        Engine_Log("Subscribe Asteroid to Ship");
        // Tell Asteroid to listen to what ship is doing 
        ship.broadcast.subscribe(&asteroids[a].onShipAction);
    }

    //Make ship send out a signal, so that the asteroids know where it is 
    //(And can move out of the way, if needed)
    ship.reset();
    Engine_Log("Ship transmit Spawn-Event");
    ship.broadcast.transmit(bceSpawn, &ship);

    Engine_Log("Subscript Ship to Engine");
    mainGameBroadcast.subscribe(&ship.heartbeat);



    return 0;
}

void GameStateMainGame::onEnd() {

    Engine_Log("unsubscribe ship from engine");
    mainGameBroadcast.unsubscribe(&ship.heartbeat);

    for(int a = 0; a < MAX_ASTEROIDS; a++) {
        Engine_Log("unsubscribe asteroid from ship");
        ship.broadcast.unsubscribe(&asteroids[a].onShipAction);
        Engine_Log("kill asteroid");
        asteroids[a].kill(&mainGameBroadcast);
    }

    Engine_Log("Unsubscribe Asteroid from Shot");
    Shot::broadcast.unsubscribe(&Asteroid::onShotAction);

    Engine_Log("Unsubscribe Shot from Engine");
    mainGameBroadcast.unsubscribe(&Shot::heartbeat);

    Engine_Log("Unsubscribe AsteroidBroadcast from Engine");
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

