#ifndef soft_gestures_h
#define soft_gestures_h

#include "Arduino.h"

class GestureListener
{
    public:
        GestureListener(int pin, int gestureFlags);
        int readRaw();
        bool read();
        Gesture getGesture();
        static const byte SWIPE = 0;
        static const byte RUB = 1;
        static const byte SWIPE_HOLD = 2;
        static const byte BUTTON = 3;
        static const byte SWIPE_FLAG = 0b1;
        static const byte RUB_FLAG = 0b10;
        static const byte BUTTON_FLAG = 0b100;
        static const byte SWIPE_HOLD_FLAG = 0b1000;
    private:
        int _pin;
        Vector<int> _keyPts;
        Gesture gestures;
        byte _flags;
}

struct Gesture{
    byte type;
    int beginValue;
    int endValue;
    int duration;
}

#endif