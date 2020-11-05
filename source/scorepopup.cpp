#include "scorepopup.h"
#include "broadcastchannels.h"
#include "../modules/RGNDS_Engine/engine.h"

#include <cstdio>

ScorePopup::ScorePopup(short s, float x, float y) {
   score = s;       
   lifetime = 45;   //45 ticks = 1.5 Sec @ 30 FPS
    pos.x = x;
    pos.y = y;
    bIsAlive = true;
}

bool ScorePopup::bDirty = false;
void ScorePopup::refreshInstanceList() {
    if(bDirty) {
        ScorePopup* sp;
        for(int a = _instances.size()-1; a >= 0; a--) {
            sp = _instances.at(a);

            if(sp->isAlive()) continue;

            delete sp;
            _instances.erase(_instances.begin()+a);
        }

        bDirty = false;
    }
}

ScorePopup::~ScorePopup() {}

std::vector<ScorePopup*> ScorePopup::_instances = std::vector<ScorePopup*>();

static int color[2] = {
    Engine_Color16(1, 31, 0, 0),
    Engine_Color16(1, 0, 31, 0)
};

void ScorePopup::cleanup(){
    for(auto s : _instances)
        delete s;

    _instances.clear();
}

ScorePopup* ScorePopup::spawn(short score, float x, float y) {

    Engine_Log("ScorePopup: " << score << " at " << x << "x" << y);

    ScorePopup* s = new ScorePopup(score, x, y);
    _instances.push_back(s);

    return s;
}

void ScorePopup::onUpdate(SpaceObj::MainGameUpdateData* data) {  
    this->lifetime--;
    if(this->lifetime==0)
        this->kill();
}

void ScorePopup::kill() {
    SpaceObj::kill();
    this->bDirty = true;
}

void ScorePopup::onDraw(SpaceObj::MainGameDrawData* data) {
    char buffer[7];
    sprintf(buffer, "%d", this->score);
    this->pos.y -= 0.25;
    RGNDS::GL2D::glText(buffer, color[this->lifetime&1], this, ((this->lifetime/45.0f)) * 31, 3); 
};