#include "Arduino.h"
#include "soft_gestures.h"
#include <stdlib.h>

#define BUFFERLOW 5

// TODO Remove define and allow programmer to define value
#define BUFFERREGION 100
#define SPIKEREGION 200
#define BUFFERHOLD 2000

/**
 * Initialize a GestureListener object to listen on a designated pin,
 * and get it to listen for specified gesture
 * @param pin designated pin to listen on
 * @param gestureFlags designates which gestures to listen to.
 */
GestureListener::GestureListener(byte pin){
    _pin = pin;
}

bool GestureListener::read(){
    int value = analogRead(_pin);
    if ((finishedGesture || current.type <= 0) && value > BUFFERLOW){
        finishedGesture = false;
        current.type = GestureListener::BUTTON;
        current.beginValue = value;
        current.endValue = value;
        current.duration = millis();
        current.hold = false;
    } else if (current.type > 0 && abs(value - current.endValue) > BUFFERREGION && value > BUFFERLOW){
        if(abs(current.endValue - value) > SPIKEREGION && current.beginValue == current.endValue){
            current.beginValue = value;
        }else if(abs(current.endValue - value) < SPIKEREGION){
            if(current.type == GestureListener::BUTTON){
                current.type = GestureListener::SWIPE;
            }
            current.endValue = value;
        }
    } else if (current.type > 0 && value < BUFFERLOW){
        finishedGesture = true;
    }
    if(current.type > 0 && !current.hold && millis() - current.duration > BUFFERHOLD){
        current.hold = true;
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