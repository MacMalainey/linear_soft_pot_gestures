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
        GestureListener(byte pin);
        int readRaw();
        bool read();
        Gesture getGesture();
        static const byte SWIPE = 1;
        static const byte BUTTON = 2;
    private:
        static int sgn(int number);
        Gesture current{0, 0, 0, 0, false};
        bool finishedGesture;
        byte _pin;
};
#endif