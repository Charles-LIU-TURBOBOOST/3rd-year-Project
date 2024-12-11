#include <Wire.h>
#include "Arduino.h"
#include "SI114X.h"

SI114X SI1145 = SI114X();

//UV Sensor global variable
const int UV_sensor_switch = 2; 
bool UV_sensor_state = LOW;
bool UV_sensor_last_sate = HIGH;
unsigned long UV_last_update_time = 0;
const unsigned long UV_update_time = 10000;
int UV_warning_count = 0;
int UV_mid_count = 0;
float UV_index;
// UV sensor global variable


void setup() 
{
  Serial.begin(115200);
  UV_Sensor_Setup();
}

void loop() 
{
  UV_Sensor_Read();
}


void UV_Sensor_Setup(){
  pinMode(UV_sensor_switch, INPUT_PULLUP);
  Serial.println("Beginning Si1145!");
  while (!SI1145.Begin()) {
      Serial.println("Si1145 is under iniliaztion!, Please wait");
      delay(2000);
  }
  Serial.println("Si1145 is ready to be used");
}

void UV_Sensor_Read(){
  UV_sensor_state = digitalRead(UV_sensor_switch);
  if (UV_sensor_state == HIGH && UV_sensor_last_sate == LOW)
  {
    Serial.println("**************************************");
    Serial.print("UV index: ");  
    UV_index = (float)SI1145.ReadUV() / 100;
    Serial.print(UV_index);
    Serial.println("(Manually Updating)");
    if (UV_index >= 8){
      Serial.println("!!! WARNING: High UV Index detected, please stay avoid outdoor exercise now !!!");
    }
    else if (UV_index < 8 && UV_index >= 3)
    {
      Serial.println("!!! ATTENTION: Please pay attention to UV radiation !!!");
    }
    delay(200);
  }
  UV_sensor_last_sate = UV_sensor_state;

  unsigned long UV_current_time = millis();
  if (UV_current_time - UV_last_update_time >= UV_update_time)
  {
    UV_last_update_time = UV_current_time;
    Serial.println("//////////////////////////////////////");
    Serial.print("UV index: ");  
    UV_index = (float)SI1145.ReadUV() / 100;
    Serial.print(UV_index);
    Serial.println("(Auto Updating)");
    if (UV_index >= 8)
    {
      UV_warning_count++;
      if (UV_warning_count >= 3){
        Serial.println("!!! WARNING: High UV Index detected, please stay avoid outdoor exercise now !!!");
      }
    }
    else if (UV_index < 8 && UV_index >= 3)
    {
      UV_warning_count = 0;
      UV_mid_count++;
      if (UV_mid_count >= 3){
        Serial.println("!!! ATTENTION: Please pay attention to UV radiation !!!");
      }
    }
    else
    {
      UV_warning_count = 0;
      UV_mid_count = 0;
    }
  }
}