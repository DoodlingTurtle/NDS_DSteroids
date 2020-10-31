#ifndef __ScorePopup_H__
#define __ScorePopup_H__

#include <vector>
#include <functional>

#include "../modules/RGNDS_Engine/gl2d.h"
#include "../modules/RGNDS_Engine/broadcast.h"
#include "../modules/RGNDS_Engine/transform.h"


class ScorePopup {
public:

    static void cleanup();
    static void spawn(short score, float x, float y);

    RGNDS::Broadcast* mainGameBroadcast;

    ScorePopup(short score, float x, float y);
    virtual~ScorePopup();

    static std::function<void(int, void*)> heartbeat;

protected:
    short score;        // Keeps track of what score is displayed
    byte lifetime;      // for how many ticks this popup will stay 
    

    RGNDS::Transform tra;


private:
    static std::vector<ScorePopup*> _instances;

};

#endif
