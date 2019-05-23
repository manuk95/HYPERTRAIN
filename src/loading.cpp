#include <Arduino.h>
#include <header.h>

/****************************************************************************
 * LAST AUFLADEN
 * initLastMotor() bringt den Aufnahmemechanismus in die Ausgangsstellung.
 * 
 * load()   Wenn der Endschalter betätigt wird, startet der Motor.
 *          Nach zwei gemessenen Umdrehungen ist die Last aufgeladen
 *          und der Motor schaltet aus. 
 ****************************************************************************/

void load(){
  beschleunigen(25);
  unsigned long beschl_time = millis();
      #ifdef DEBUG_
          Serial.println("Beschleunigung 25");
          Serial.println("Warten auf Endschalter..");
      #endif
  
    unsigned long load_wait_start = millis();
  while(digitalRead(ENDSCHALTER_PIN) && WAIT_WHILE(load_wait_start, 7000));
  //while(digitalRead(ENDSCHALTER_PIN));
  beschl_time = millis() - beschl_time;
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

  load_wait_start = millis();
 // while(!digitalRead(HALL_LAST_2_PIN) && WAIT_WHILE(load_wait_start, 6000));
  while(digitalRead(HALL_LAST_2_PIN));
    #ifdef DEBUG_
          Serial.println("Hallsensor2 on!");
    #endif

  load_wait_start = millis();
  //while(!digitalRead(HALL_LAST_2_PIN) && WAIT_WHILE(load_wait_start, 500));
  while(!digitalRead(HALL_LAST_2_PIN));
      #ifdef DEBUG_
          Serial.println("Hallsensor2 off!");
          Serial.println("Warten auf Hallsensor2..");
      #endif

  load_wait_start = millis();
 // while(!digitalRead(HALL_LAST_2_PIN) && WAIT_WHILE(load_wait_start, 6000));
  
  while(digitalRead(HALL_LAST_2_PIN));

      #ifdef DEBUG_
          Serial.println("Hallsensor2 on");
      #endif

  digitalWrite(MOT_LAST_PIN, LOW);
      #ifdef DEBUG_
          Serial.println("Lastmotor aus!");
      #endif
  check_init_lastmotor = false;

    
    beschleunigen(-25);
    load_wait_start = millis();
    while(WAIT_WHILE(load_wait_start, beschl_time));
    beschleunigen(0);
    rot_count = 0;

}

void initLastMotor(){
   // long initL_start = millis();
    digitalWrite(MOT_LAST_PIN, HIGH);
    delay(300);
    analogWrite(MOT_LAST_PIN, 100);
    while(digitalRead(HALL_LAST_1_PIN));
    digitalWrite(MOT_LAST_PIN, LOW);
    check_init_lastmotor = true;

}