#include "Arduino.h"
#include "soft_gestures.h"

GestureListener::GestureListener(int pin, byte gestureFlags){
    _pin = pin;
    _flags = gestureFlags;
}

bool GestureListener::read(){
    analogRead(_pin);
}

int GestureListener::readRaw(){
    return analogRead(_pin);
}

