#include <Arduino.h>
#include <header.h>

/********************************************************
 * LAST AUFLADEN
 * initLastMotor bringt den Aufnahmemechanismus in die richtige Stellung.
 * 
 * Wenn der Endschalter betätigt wird, start_isret der Motor.
 * Nach zwei gemessenen Umdrehungen ist die Last aufgeladen
 * und der Motor schaltet aus. 
 ********************************************************/

void load(){
  beschleunigen(25);
      #ifdef DEBUG_
          Serial.println("Beschleunigung 25");
          Serial.println("Warten auf Endschalter..");
      #endif
  

  while(digitalRead(ENDSCHALTER_PIN));
  beschleunigen(0);
     #ifdef DEBUG_
          Serial.println("Endschalter gedrückt.");
          Serial.println("Beschleunigung 0");
      #endif

  digitalWrite(MOT_LAST_PIN, HIGH);
      #ifdef DEBUG_
          Serial.println("Last Motor ein");
          Serial.println("Warten auf Hallsensor2..");
      #endif
  
  while(digitalRead(HALL_LAST_2_PIN));
    #ifdef DEBUG_
          Serial.println("Hallsensor2 on!");
    #endif

  while(!digitalRead(HALL_LAST_2_PIN));
      #ifdef DEBUG_
          Serial.println("Hallsensor2 off!");
          Serial.println("Warten auf Hallsensor2..");
      #endif

  while(digitalRead(HALL_LAST_2_PIN));
      #ifdef DEBUG_
          Serial.println("Hallsensor2 on");
      #endif

  digitalWrite(MOT_LAST_PIN, LOW);
      #ifdef DEBUG_
          Serial.println("Lastmotor aus!");
      #endif
  
}

void initLastMotor(){
  long initL_start = millis();
  digitalWrite(MOT_LAST_PIN, HIGH);
  while(digitalRead(HALL_LAST_1_PIN) && WAIT_WHILE(initL_start, 6000));
  digitalWrite(MOT_LAST_PIN, LOW);

}