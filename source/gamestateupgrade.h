#ifndef __GameStateUpgrade_H__
#define __GameStateUpgrade_H__

#include "../modules/RGNDS_Engine/engine.h"
#include "../modules/RGNDS_Engine/addons/TextMenu/textmenu.h"

#include "shipstats.h"

class GameStateUpgrade : public RGNDS::Engine {
public:
    GameStateUpgrade(ShipStats *shipstats);

    void onDraw(RGNDS::Engine::Screen screen);

protected:

    int onStart();
    void onUpdate(float deltaTime);

private:
    ShipStats *shipstats;

    RGNDS::TextMenu *selection;

};

#endif
