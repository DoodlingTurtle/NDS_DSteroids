#ifndef __GameStateUpgrade_H__
#define __GameStateUpgrade_H__

#include <functional>
#include <vector>

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "shipstats.h"

class GameStateUpgrade : public RGNDS::Engine {
public:
    GameStateUpgrade(ShipStats *shipstats);

    void onDraw(RGNDS::Engine::Screen screen);
    int* score;
    float* game_difficulty; 

protected:
    int onStart();
    void onEnd();
    void onUpdate(float deltaTime);

private:
    ShipStats *shipstats;
    RGNDS::TextMenu selection;

    std::vector<char*> description;
    RGNDS::Transform descriptionlocation;
    RGNDS::Transform scorelocation;
    RGNDS::Transform costlocation;

    bool showError;

    std::vector<int> upgrade_data;
    std::function<void()> handlers[4];
};

#endif
