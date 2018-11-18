#include "Arduino.h"
#include "soft_gestures.h"
#include <stdlib.h>

#define BUFFERLOW 5

// TODO Remove define and allow programmer to define value
#define BUFFERREGION 100
#define BUFFERHOLD 2000

/**
 * Initialize a GestureListener object to listen on a designated pin,
 * and get it to listen for specified gesture
 * @param pin designated pin to listen on
 * @param gestureFlags designates which gestures to listen to.
 */
GestureListener::GestureListener(byte pin, byte flags){
    _pin = pin;
    _flags = flags;
}

bool GestureListener::read(){
    uint8_t value = analogRead(_pin);
    if ((finishedGesture || current.type <= 0) && value > BUFFERLOW){
        finishedGesture = false;
        current.beginValue = value;
        current.endValue = value;
        current.type = GestureListener::BUTTON;
        current.duration = millis();
    } else if (current.type > 0 && abs(value - current.endValue) > BUFFERREGION && value > BUFFERLOW){
        if(current.type != 2){
            if(current.type == GestureListener::BUTTON){
                current.type = GestureListener::SWIPE;
            }
        }
        current.endValue = value;
    } else if (current.type > 0 && value < BUFFERLOW){
        finishedGesture = true;
    }
    return finishedGesture;
}

int GestureListener::readRaw(){
    return analogRead(_pin);
}

Gesture GestureListener::getGesture(){
    finishedGesture = false;
    Gesture cacheGesture = current;
    cacheGesture.duration = millis() - current.duration;
    if(!cacheGesture.hold){
        current = {0, 0, 0, 0};
    }
    return cacheGesture;
}