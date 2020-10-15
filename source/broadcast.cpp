#include "broadcast.h"

std::map<const int, Broadcast*> Broadcast::channels;

Broadcast::Broadcast(const int channel ) {
    if(channel < 0)
        return;
    if(channels.find(channel) != channels.end())
        return;

    channels[channel] = this;
    this->channel = channel;
}

Broadcast::~Broadcast(){
    if(channel > -1)
        channels.erase(channels.find(channel));
}

void Broadcast::transmit(int event, void* broadcaster) {

    for(Broadcast::Listener* l : listeners)
        l->onBroadcast(channel, event, broadcaster);

}

bool Broadcast::subscribe(Listener* l) {
    listeners.push_back(l);
    return true;
}

bool Broadcast::unsubscribe(Listener* l) {

    for(auto a = listeners.begin(); a != listeners.end(); ++a ) {
        if(*a == l) {
            listeners.erase(a);
            return true;
            break;
        }
    }

    return false;
}

void Broadcast::Listener::onBroadcast(int c, int e, void* b) {}
