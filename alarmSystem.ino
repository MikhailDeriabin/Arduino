#include "IRremote.h"
#include <Servo.h>

float minDistance = 20;

int currentAngle = 90;
int minAngle = 20;
int maxAngle = 160;
//true = left, false = right
bool moveSide = true;

IRrecv iRReciver(8);
decode_results pressedKeys;
#define POWER_BUTTON_CODE 0xFFA25D

Servo myservo;

void moveServo(){  
  if(moveSide){
    currentAngle += 2;
    if(currentAngle >= maxAngle)
      moveSide = false;
  } else{
    currentAngle -= 2;
    if(currentAngle <= minAngle)
      moveSide = true;
  }
  myservo.write(currentAngle);  
  delay(30);
}

void stopServo(){
  myservo.write(90);
  myservo.detach();
}

void setWarningMode(){
  myservo.detach();
  while(measureDistance() < minDistance){
    Serial.println(measureDistance());
    //Turn on warning LED
    digitalWrite(5, HIGH); 
    delay(100); 
  }
  //Turn off warning LED
  digitalWrite(5, LOW);
  
  myservo.attach(9);
}

float measureDistance(){
    digitalWrite(7, LOW); 
    delayMicroseconds(2); 
    digitalWrite(7, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(7, LOW);
    
    return (pulseIn(4, HIGH)*0.0343)/2;
}

void setup(){
  iRReciver.enableIRIn();

  // power indicator LED
  pinMode(6, OUTPUT);
  digitalWrite(7, LOW);

  // connect US sensor
  pinMode(7, OUTPUT); //trig
  pinMode(4, INPUT);  //echo

  pinMode(5, OUTPUT); //warning LED and active buzzer

  Serial.begin(9600);  
  Serial.println("Program started");
}

boolean isSystemOn = false;

void loop(){
  if(iRReciver.decode(&pressedKeys)){
    if(pressedKeys.value == POWER_BUTTON_CODE){
      if(!isSystemOn){
        digitalWrite(6, HIGH);
        myservo.attach(9);
        Serial.println("System is on");
        isSystemOn = true;
      } else{
        digitalWrite(6, LOW);
        stopServo();
        Serial.println("System is off"); 
        isSystemOn = false;
      }         
    }

    iRReciver.resume();
  }

  if(isSystemOn){
    moveServo(); 

    float distance = measureDistance();
    if(distance < minDistance){      
      setWarningMode(); 
    }
  }
}    
