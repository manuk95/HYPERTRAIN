#include <Arduino.h>
#include <header.h>

/********************************************************
 * Hier werden alle globalen Variabeln definiert
 * 
 * 
 * 
 * 
 *  
 ********************************************************/

int rot_count;
long start_isr;
long start_race;
double set_speed, cur_speed, Output;

state_t state;
modus_t modus;

void initSetupVariables()
{
    rot_count = 0;
    Output = 0;
    set_speed = 0;
    modus = MODE_NORMAL;
    state = WAIT;
}

void pinModeSetup()
{

  pinMode(IR_PIN, INPUT);
  pinMode(U_GLEIS_PIN, INPUT);
  pinMode(TACHO_PIN, INPUT);
  pinMode(HALL_LAST_1_PIN, INPUT);
  pinMode(HALL_LAST_2_PIN, INPUT);
  pinMode(ENDSCHALTER_PIN, INPUT); 

  pinMode(HBRI_F_PIN, OUTPUT);
  pinMode(HBRI_R_PIN, OUTPUT);
  pinMode(MOT_LAST_PIN, OUTPUT);
  pinMode(USV_DIS_PIN, OUTPUT);
  pinMode(LED_VORNE_PIN, OUTPUT);
  pinMode(LED_HINTEN_PIN, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(TACHO_PIN), isr_tacho_count, RISING);
}