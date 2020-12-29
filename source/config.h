#ifndef __MainConfig_H__
#define __MainConfig_H__

#include <nds.h>

#define MAX_ASTEROIDS 48
#define CNT_STARS 64

enum GameInput {
    GAMEINPUT_ACCELERATE,
    GAMEINPUT_TURNLEFT,
    GAMEINPUT_TURNRIGHT,
    GAMEINPUT_FIRE
};

enum KeyPadInput {
    KEYPAD_LEFT,
    KEYPAD_RIGHT,
    KEYPAD_UP,
    KEYPAD_DOWN,
    KEYPAD_SELECT,
    KEYPAD_A,
    KEYPAD_B,
    KEYPAD_X,
    KEYPAD_Y,
    KEYPAD_L,
    KEYPAD_R
};

extern int GameKeyMap[11];

#endif
