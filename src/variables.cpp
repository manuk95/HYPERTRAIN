#include <Arduino.h>
#include <header.h>

/********************************************************
 * Globale Variabeln deklarieren und initialisieren
 * allg. Setup-Funktionen implementieren
 * 
 ********************************************************/

int rot_count;
long start_isr;
long start_race;
long start_acc;

double set_speed, cur_speed, Output;
bool check_init_lastmotor;
int last_step;

state_t state;
modus_t modus;

void initSetupVariables()
{
    rot_count = 0;
    Output = 0;
    set_speed = 0;
    modus = MODE_NORMAL;
    state = WAIT;
    check_init_lastmotor = false;
    last_step = -1;
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

void setLEDstate()
{
  digitalWrite(LED_VORNE_PIN, HIGH); 
  digitalWrite(LED_HINTEN_PIN, LOW);
}

