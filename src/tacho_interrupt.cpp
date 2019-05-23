#include <Arduino.h>
#include <header.h>

/********************************************************
 * INTERRUPT
 * Funktion wird durch den INterrupt ausgelöst.
 *                     
 *
 * 
 * 
 ********************************************************/

void isr_tacho_count()
{
  rot_count++;

  get_speed();
  readData();

#ifdef DEBUG_
  Serial.print("ROT_COUNT: ");
  Serial.println(rot_count);
#endif

#ifdef TEST_ROT
  digitalWrite(LED_HINTEN_PIN, !digitalRead(LED_HINTEN_PIN));
#endif

  if (acc_modus == MODE_PID)
  {
    PIDupdate();
  }


/*
  if (rot_count >= MAX_ROT_COUNT)
  {
    rot_count = 0;
    if (state == DRIVE || state == ACCELERATION)
    {
      if (Output > 25)
      {
        beschleunigen(25);
      }
      state = APPROACHSTOP;
    }
  }
  */

  if (state == STOPPING)
  {
    last_step -= (WHEEL_CIRC / ANZAHL_MAGNETE / 10); // last_step in mm
#ifdef DEBUG_
    Serial.print("LAST_STEP: ");
    Serial.println(last_step);
#endif
    if (last_step < WHEEL_CIRC / ANZAHL_MAGNETE / 10)
    {
      beschleunigen(0);
      state = FINISH;
      sendJson("FINISH", last_step);
    }
  }
}