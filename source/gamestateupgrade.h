#ifndef __GameStateUpgrade_H__
#define __GameStateUpgrade_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "shipstats.h"

class GameStateUpgrade : public RGNDS::Engine {
public:
    GameStateUpgrade(ShipStats *shipstats);

    void onDraw(RGNDS::Engine::Screen screen);
    int* score;

protected:
    int onStart();
    void onEnd();
    void onUpdate(float deltaTime);

private:
    ShipStats *shipstats;
    RGNDS::TextMenu selection;

    char* description[4];
    RGNDS::Transform descriptionlocation;
    RGNDS::Transform scorelocation;

    bool showError;
};

#endif
