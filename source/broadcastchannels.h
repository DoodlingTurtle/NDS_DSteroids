#ifndef __BROADCASTCHANNELS_H__
#define __BROADCASTCHANNELS_H__

enum BroadcastChannel{
    bchShip = 0,
    bchAsteroid,
    bchShot
};

enum BroadcastEvent {
    bceMove = 0,
    bceDead,
    bceSpawn,
    bceHitPlayer
};

#endif // __BROADCASTCHANNELS_H__
