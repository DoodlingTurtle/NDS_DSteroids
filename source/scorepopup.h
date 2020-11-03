#ifndef __ScorePopup_H__
#define __ScorePopup_H__

#include <vector>
#include <functional>

#include "../modules/RGNDS_Engine/gl2d.h"
#include "../modules/RGNDS_Engine/broadcast.h"
#include "../modules/RGNDS_Engine/transform.h"

#include "spaceobj.h"


class ScorePopup : public SpaceObj {
public:

    static void cleanup();
    static ScorePopup* spawn(short score, float x, float y);

    /** \brief deletes all dead instances from the memeory */
    static void refreshInstanceList();

    RGNDS::Broadcast* mainGameBroadcast;

    ScorePopup(short score, float x, float y);
    virtual~ScorePopup();

    void onUpdate(SpaceObj::MainGameUpdateData*);
    void onDraw(SpaceObj::MainGameDrawData*);

    void kill();

protected:
    short score;        // Keeps track of what score is displayed
    byte lifetime;      // for how many ticks this popup will stay 

private:
    static std::vector<ScorePopup*> _instances;

    static bool bDirty;

};

#endif
