#ifndef __BROADCASTCHANNELS_H__
#define __BROADCASTCHANNELS_H__

enum BroadcastEvent {
    bceMove = 0,
    bceDead,
    bceSpawn,
    bcePlayerShotsFired,
    bceHitPlayer,
    bceGameStart,
    bceTick,
    bceDraw
};

#endif // __BROADCASTCHANNELS_H__
