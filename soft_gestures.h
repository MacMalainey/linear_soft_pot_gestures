#ifndef soft_gestures_h
#define soft_gestures_h

#include "Arduino.h"
#include <stdlib.h>

struct Gesture
{
    byte type;
    int beginValue;
    int endValue;
    unsigned long duration;
    bool hold;
};

class GestureListener
{
    public:
        GestureListener(byte pin, byte flags);
        int readRaw();
        bool read();
        Gesture getGesture();
        static const byte SWIPE = 1;
        static const byte RUB = 2;
        static const byte BUTTON = 3;
        static const byte SWIPE_FLAG = 0b1;
        static const byte RUB_FLAG = 0b10;
        static const byte BUTTON_FLAG = 0b100;
        static const byte SWIPE_HOLD_FLAG = 0b1000;
    private:
        Gesture current{0, 0, 0, 0, false};
        bool finishedGesture;
        byte _pin;
        byte _flags;
};
#endif