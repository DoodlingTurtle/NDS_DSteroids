#ifndef __BROADCASTCHANNELS_H__
#define __BROADCASTCHANNELS_H__

enum BroadcastChannel{
    bchShip = 0,
    bchAsteroid
};

enum BroadcastEvent {
    bceMove = 0,
    bceDead,
    bceHitPlayer
};

#endif // __BROADCASTCHANNELS_H__
