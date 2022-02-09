#include "IRremote.h"
#include "dht11.h"

const int IR_REC_PIN = 3;
const int POWER_LED_PIN = 4;
const int TEMP_SENSOR_PIN = 5;
const int VENTILATOR_ENABLE_PIN = 7;
const int VENTILATOR_SPIN_DIR1_PIN = 8;

IRrecv iRReciver(IR_REC_PIN);
decode_results pressedKeys;
#define POWER_BUTTON_CODE 0xFFA25D

dht11 tempSensor;
const float MAX_TEMPERATURE = 27.0;

void setup() {
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, LOW);

  pinMode(VENTILATOR_ENABLE_PIN, OUTPUT);
  pinMode(VENTILATOR_SPIN_DIR1_PIN, OUTPUT);

  iRReciver.enableIRIn();

  Serial.begin(9600);  
  Serial.println("Program started");
}

boolean isSystemOn = false;
boolean isVentilatorOn = false;
void loop() {
  if(iRReciver.decode(&pressedKeys)){
    if(pressedKeys.value == POWER_BUTTON_CODE){
      if(!isSystemOn){
        digitalWrite(POWER_LED_PIN, HIGH); 
        Serial.print("System is on. Maximal temperature is set up to "); 
        Serial.println(MAX_TEMPERATURE);
        Serial.println();
        isSystemOn = true;
      } else{
        digitalWrite(POWER_LED_PIN, LOW); 
        digitalWrite(VENTILATOR_ENABLE_PIN, LOW);
        digitalWrite(VENTILATOR_SPIN_DIR1_PIN, LOW);
        Serial.println("System is off"); 
        isSystemOn = false;
        isVentilatorOn = false;
      }     
      
    }
    
    iRReciver.resume();
  }

  if(isSystemOn){     
    tempSensor.read(TEMP_SENSOR_PIN);
    float currentTemperature = tempSensor.temperature;
    
    if(currentTemperature >= MAX_TEMPERATURE && !isVentilatorOn){
      digitalWrite(VENTILATOR_ENABLE_PIN, HIGH);
      digitalWrite(VENTILATOR_SPIN_DIR1_PIN, HIGH);
      Serial.println("Temperature is high. Ventelator is on");
      isVentilatorOn = true;          
    } else if(currentTemperature < MAX_TEMPERATURE && isVentilatorOn){
      digitalWrite(VENTILATOR_ENABLE_PIN, LOW);
      digitalWrite(VENTILATOR_SPIN_DIR1_PIN, LOW);
      Serial.println("Temperature is now normal. Ventelator is off");
      isVentilatorOn = false;         
    }
  }
 
}
