#include "Arduino.h"
#include "soft_gestures.h"
#include <stdlib.h>

#define BUFFERLOW 5

// TODO Remove define and allow programmer to define value
#define BUFFERREGION 150
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
    int value = analogRead(_pin);
    if ((finishedGesture || current.type <= 0) && value > BUFFERLOW){
        finishedGesture = false;
        current.beginValue = value;
        current.endValue = value;
        current.type = GestureListener::BUTTON;
        current.duration = millis();
        current.hold = false;
    } else if (current.type > 0 && abs(value - current.beginValue) > BUFFERREGION && value > BUFFERLOW){
        if(current.type != GestureListener::RUB){
            if(current.type == GestureListener::BUTTON){
                current.type = GestureListener::SWIPE;
            }
        }
        if(current.type == GestureListener::SWIPE && 
            GestureListener::sgn(value - current.endValue) == 
            GestureListener::sgn(current.beginValue - current.endValue) && 
            !current.hold){
            current.type = GestureListener::RUB;
        }
        current.endValue = value;
    } else if (current.type > 0 && value < BUFFERLOW){
        finishedGesture = true;
    }
    if(current.type > 0 && !current.hold && millis() - current.duration > BUFFERHOLD){
        current.hold = true;
        if(current.type == GestureListener::RUB){
            current.type = GestureListener::SWIPE;
        }
    }
    return finishedGesture || current.hold;
}

int GestureListener::readRaw(){
    return analogRead(_pin);
}

Gesture GestureListener::getGesture(){
    Gesture cacheGesture = current;
    cacheGesture.duration = millis() - current.duration;
    if(finishedGesture){
        current = {0, 0, 0, 0, false};
    }
    finishedGesture = false;
    return cacheGesture;
}

int GestureListener::sgn(int number) {
  if (number < 0) return -1;
  if (number > 0) return 1;
  return 0;
}