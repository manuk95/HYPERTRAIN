#include <Arduino.h>
#include <header.h>



/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  SHARP IR Sensor  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_gp2d12 () {
  uint16_t value = analogRead (IR_PIN);
  uint16_t local_distanz = ((67870.0 / (value - 3.0)) - 40.0);
  if (value < 10) value = 10;
  return  local_distanz; // Distanz in mm
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  COUNT  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void isr_tacho_count(){
  rot_count++;
  get_speed();
  
  #ifdef TEST_ROT
    digitalWrite(LED_HINTEN_PIN, !digitalRead(LED_HINTEN_PIN));
  #endif
  
  if(modus == MODE_PID) {PIDupdate();}
  if(rot_count >= MAX_ROT_COUNT)
  {
    rot_count = 0;
    if(state == DRIVE || state == ACCELERATION) 
    {
      if(Output > 25) {beschleunigen(25);}
      state = APPROACHSTOP;
    }
  }
  if(state == STOPPING)
  {
      last_step -= WHEEL_CIRC/ANZAHL_MAGNETE * 10;
      if(last_step < WHEEL_CIRC/ANZAHL_MAGNETE){
        beschleunigen(0);
        state = FINISH;
        sendJson("FINISH", last_step);
      }
  }
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  GET SPEED  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void get_speed(){
  float dauer = millis() - start_isr;
  start_isr = millis();
  cur_speed = (double) (WHEEL_CIRC/(ANZAHL_MAGNETE*10)) / (dauer /1000);           // mm/s
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** DISTANZ  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_distanz()
{
  uint16_t range = get_gp2d12();
  uint16_t distanz = range * cos(radiant());
  
  #ifdef DEBUG_
   // Serial.print("Range: ");    Serial.print(range);      Serial.println(" mm");
   //  Serial.print("Distanz: ");  Serial.print(distanz);    Serial.println(" mm");
  #endif

  #ifdef TEST_
    if(range < 100)
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
  #endif

  if(distanz < 100)
  {
    return distanz;
  }
  distanz = 0;
  return distanz;
}

double radiant()
{
  return (double) (IR_WINKEL / 360 * 2 * 3.14);
}

uint16_t getLastStep()
{
  while((int)0 == (int)get_distanz());
  delay(20);
  if(get_distanz() != 0){
    return get_distanz();
  }
  else
  {
    return 0;
  }
  
}


/********************************************************
 * CHECK TIME
 * 
 ********************************************************/
void checkTime()
{
  if((millis() - start_race) > 130000){
    if(state < 3){
      state = APPROACHSTOP;
    }
  }
  else if((millis() - start_race) > 240000)
  {
    beschleunigen(0);
    state = FINISH;
  }

  #ifndef TEST_
    if((millis() % 1000 == 0))
    {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
  #endif
}