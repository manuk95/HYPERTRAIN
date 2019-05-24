#include <Arduino.h>
#include <header.h>



/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  SHARP IR Sensor  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_gp2d12 () {
  uint16_t value = analogRead (IR_PIN);
  uint16_t local_distanz = ((67870.0 / (value - 3.0)) - 40.0);
  if (value < 10) value = 10;
  return  local_distanz; // Distanz in mm
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
  uint16_t distanz = range; //* cos(radiant());
  
  #ifdef DEBUG_
   // Serial.print("Range: ");    Serial.print(range);      Serial.println(" mm");
   //  Serial.print("Distanz: ");  Serial.print(distanz);    Serial.println(" mm");
  #endif

  #ifdef TEST_
    if(range < 200)
    {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
  #endif

  if(distanz < 200)
  {
    return distanz;
  }
  distanz = 0;
  return distanz;
}



uint16_t getLastStep()
{
  while((int)0 == (int)get_distanz()){yield();}
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
  /*
  long runTime = millis() - start_acc;

  if(runTime > 180000){
    if(state < 4){
      beschleunigen(25);
      state = APPROACHSTOP;
      sendJson("approachstop", runTime);
    }
  }
  else if(runTime > 240000)
  {
    if(state != FINISH)
    {
      beschleunigen(0);
      state = FINISH;
      sendJson("finish", runTime);
    }
  }
*/
//rot_count += 10;
 


  
}