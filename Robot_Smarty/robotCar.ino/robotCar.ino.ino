//settings
const float frMinDist = 20;   //min distance in the front
const int movingSpeed = 255;  //0 - 255 

//pins
//L293D (DC motors)
//FLM = front left motor
const int enableFLMPin = 33;
const int in1FLMPin = 31;  // ==HIGH => spin forward
const int in2FLMPin = 29; // ==HIGH => spin back
//BLM = back left motor
const int enableBLMPin = 27;
const int in1BLMPin = 25;  // ==HIGH => spin forward
const int in2BLMPin = 23; // ==HIGH => spin back

//FRM = front right motor
const int enableFRMPin = 32;
const int in1FRMPin = 30;  // ==HIGH => spin forward
const int in2FRMPin = 28; // ==HIGH => spin back
//BRM = back right motor
const int enableBRMPin = 26;
const int in1BRMPin = 24;  // ==HIGH => spin forward
const int in2BRMPin = 22; // ==HIGH => spin back

//Ultrasonic sensor
const int usFrEchoPin = 10;    //usFr = ultasonic front
const int usFrTrigPin = 9;

//Touch sensors
const int tsRPin = 13;  //tsR = touch sensor right
const int tsCPin = 12;
const int tsLPin = 11;


//functions

//moving
//TODO: combine moveForward, moveBack and stopMotors methods
void moveForward(int movSpeed, float duration){
  moveForward(movSpeed);
  delay(duration);
  stopMotors();
}
void moveForward(int movSpeed){
  analogWrite(enableFLMPin, movSpeed);
  analogWrite(enableBLMPin, movSpeed);
  analogWrite(enableFRMPin, movSpeed);
  analogWrite(enableBRMPin, movSpeed);

  digitalWrite(in1FLMPin, HIGH);
  digitalWrite(in2FLMPin, LOW);
  digitalWrite(in1BLMPin, HIGH);
  digitalWrite(in2BLMPin, LOW);

  digitalWrite(in1FRMPin, HIGH);
  digitalWrite(in2FRMPin, LOW);
  digitalWrite(in1BRMPin, HIGH);
  digitalWrite(in2BRMPin, LOW);
}

void moveBack(int movSpeed, float duration){
  moveBack(movSpeed);
  delay(duration);
  stopMotors();
}
void moveBack(int movSpeed){
  analogWrite(enableFLMPin, movSpeed);
  analogWrite(enableBLMPin, movSpeed);
  analogWrite(enableFRMPin, movSpeed);
  analogWrite(enableBRMPin, movSpeed);

  digitalWrite(in1FLMPin, LOW);
  digitalWrite(in2FLMPin, HIGH);
  digitalWrite(in1BLMPin, LOW);
  digitalWrite(in2BLMPin, HIGH);

  digitalWrite(in1FRMPin, LOW);
  digitalWrite(in2FRMPin, HIGH);
  digitalWrite(in1BRMPin, LOW);
  digitalWrite(in2BRMPin, HIGH);
}

void turnRight90(int turnSpeed){
  analogWrite(enableFLMPin, turnSpeed);
  analogWrite(enableBLMPin, turnSpeed);
  analogWrite(enableFRMPin, turnSpeed);
  analogWrite(enableBRMPin, turnSpeed);

  digitalWrite(in1FLMPin, LOW);
  digitalWrite(in2FLMPin, HIGH);
  digitalWrite(in1BLMPin, LOW);
  digitalWrite(in2BLMPin, HIGH);

  digitalWrite(in1FRMPin, HIGH);
  digitalWrite(in2FRMPin, LOW);
  digitalWrite(in1BRMPin, HIGH);
  digitalWrite(in2BRMPin, LOW);

  delay(725);
  stopMotors();
}

void stopMotors(){
  analogWrite(enableFLMPin, 0);
  analogWrite(enableBLMPin, 0);
  analogWrite(enableFRMPin, 0);
  analogWrite(enableBRMPin, 0);

  digitalWrite(in1FLMPin, LOW);
  digitalWrite(in2FLMPin, LOW);
  digitalWrite(in1BLMPin, LOW);
  digitalWrite(in2BLMPin, LOW);

  digitalWrite(in1FRMPin, LOW);
  digitalWrite(in2FRMPin, LOW);
  digitalWrite(in1BRMPin, LOW);
  digitalWrite(in2BRMPin, LOW);
}

void turnAround(int turnSpeed){
  stopMotors();
  delay(1000);
  moveBack(turnSpeed, 300);
  delay(1000);
  turnRight90(turnSpeed);
  delay(1000);
  turnRight90(turnSpeed);
  delay(1000);
}

//sensors
float measureDistance(){
    digitalWrite(usFrTrigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(usFrTrigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(usFrTrigPin, LOW);
    
    return (pulseIn(usFrEchoPin, HIGH)*0.0343)/2;
}

bool isTouching(){
  bool isTouch = false;

  if((digitalRead(tsLPin)) == HIGH){
    isTouch = true;
  }

  return isTouch;
}

void setup() {
  //pins' modes
  pinMode(enableFLMPin, OUTPUT);
  pinMode(enableBLMPin, OUTPUT);
  pinMode(enableFRMPin, OUTPUT);
  pinMode(enableBRMPin, OUTPUT);

  pinMode(in1FLMPin, OUTPUT);
  pinMode(in2FLMPin, OUTPUT);
  pinMode(in1BLMPin, OUTPUT);
  pinMode(in2BLMPin, OUTPUT);
  
  pinMode(in1FRMPin, OUTPUT);
  pinMode(in2FRMPin, OUTPUT);
  pinMode(in1BRMPin, OUTPUT);
  pinMode(in2BRMPin, OUTPUT);
  
  pinMode(usFrEchoPin, INPUT);
  pinMode(usFrTrigPin, OUTPUT);

  Serial.begin(9600);
  delay(6000);
}

float distance = frMinDist;

int currentSpeed = movingSpeed;

void loop() {
  moveForward(currentSpeed);  
  //distance = measureDistance();
/*
  if(distance <= frMinDist+20){
    currentSpeed /= 2;
  } else{
    currentSpeed = movingSpeed;
  }
*/
  //distance <= frMinDist ||
  if(isTouching()){
    turnAround(movingSpeed);
  }
}
