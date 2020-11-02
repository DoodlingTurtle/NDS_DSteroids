#include "scorepopup.h"
#include "broadcastchannels.h"
#include "../modules/RGNDS_Engine/engine.h"

#include <cstdio>

ScorePopup::ScorePopup(short s, float x, float y) {
   score = s;       
   lifetime = 45;   //45 ticks = 1.5 Sec @ 30 FPS
    tra.pos.x = x;
    tra.pos.y = y;
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

void ScorePopup::spawn(short score, float x, float y) {

    ScorePopup* s = new ScorePopup(score, x, y);
    _instances.push_back(s);

}



std::function<void(int, void*)> ScorePopup::heartbeat = [](int event, void* data){
    switch(event) {
        case bceTick: {
            ScorePopup* s;
            for(int a = _instances.size()-1; a >= 0; a--) {
               s = _instances.at(a);

               s->lifetime--;
               if(s->lifetime==0) {
                    delete s;
                    _instances.erase(_instances.begin()+a);
               }
            }
        } break;

        case bceDraw: {
            for(auto s : _instances) {
                char buffer[7];
                sprintf(buffer, "%d", s->score);
                s->tra.pos.y -= 0.25;
                RGNDS::GL2D::glText(buffer, color[s->lifetime&1], &s->tra, ((s->lifetime/45.0f)) * 31); 
            }
        } break;
    }
};
