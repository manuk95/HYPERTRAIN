#include <Arduino.h>
#include <header.h>



/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  SHARP IR Sensor  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_gp2d12 () {
  uint16_t value = analogRead (IR_PIN);
  if (value < 10) value = 10;
  return ((67870.0 / (value - 3.0)) - 40.0); // Distanz in mm
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  COUNT  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void isr_tacho_count(){
  rot_count++;
  get_speed();
  
  if(modus == MODE_PID) {PIDupdate();}
  if(rot_count >= MAX_ROT_COUNT)
  {
    rot_count = 0;
    if(state == DRIVE || state == ACCELERATION) {state = APPROACHSTOP;}
  }
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  GET SPEED  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void get_speed(){
  float dauer = millis() - start_isr;
  start_isr = millis();
  cur_speed = (double) (WHEEL_CIRC/10) / (dauer /1000);           // mm/s
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** DISTANZ  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_distanz()
{
  uint16_t range = get_gp2d12();
  uint16_t distanz = range * cos(radiant());
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
}