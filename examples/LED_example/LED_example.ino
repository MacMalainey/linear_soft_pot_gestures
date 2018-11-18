#include <soft_gestures.h>

#define shiftPin 6
#define clockPin 8
#define holdPin 9
#define led1Pin 1
#define led2Pin 2
#define led3Pin 3

bool shiftLEDS[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
bool swipe = false;
bool button = false;
unsigned long lastButton = 0;
unsigned long lastLED = 0;

void setup() {
  pinMode(shiftPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(holdPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  Serial.begin(9600);
}

GestureListener listener(4, 0);

void loop() {
  if(listener.read()){
    Gesture gesture = listener.getGesture();
    Serial.print("Type: ");
    Serial.println(gesture.type);
    Serial.print("Begin: ");
    Serial.println(gesture.beginValue);
    Serial.print("End: ");
    Serial.println(gesture.endValue);
    Serial.print("Duration: ");
    Serial.println(gesture.duration);
    swipe = gesture.type == GestureListener::SWIPE;
    button = gesture.type == GestureListener::BUTTON;
  }
  if(swipe && millis() - lastLED > 100){
    bool allTrue = true;
    for(int i = 0; i < 8; i++){
      if(!shiftLEDS[i]){
        shiftLEDS[i] = true;
        allTrue = false;
        break;
      }
    }
    if(allTrue){
      swipe = false;
      shiftLEDS[0] = false;
      shiftLEDS[1] = false;
      shiftLEDS[2] = false;
      shiftLEDS[3] = false;
      shiftLEDS[4] = false;
      shiftLEDS[5] = false;
      shiftLEDS[6] = false;
      shiftLEDS[7] = false;
    }
    digitalWrite(holdPin, LOW);
    shiftOut(shiftPin, clockPin, MSBFIRST, shift_array());
    digitalWrite(holdPin, HIGH);
    lastLED = millis();
  }
}

int shift_array(){
  return shiftLEDS[0] * 1 +
    shiftLEDS[1] * 2 + 
    shiftLEDS[2] * 4 +
    shiftLEDS[3] * 8 +
    shiftLEDS[4] * 16 + 
    shiftLEDS[5] * 32 + 
    shiftLEDS[6] * 64 +
    shiftLEDS[7] * 128;
}

