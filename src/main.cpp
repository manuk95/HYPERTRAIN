#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>

state_t state;
modus_t modus;

void setup() {
  Serial.begin(9600);

  /*
  * Festlegen der PIN-Modes
  */ 
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

  PIDOutputLimit();

  rot_count = 0;
  Output = 0;
  
  set_speed = 0;
  modus = MODE_NORMAL;
  state = WAIT;
  digitalWrite(LED_BUILTIN, HIGH); 
  
  sendJson("setup", 1);    // Test
}



void loop() {

  if(state == WAIT){
    readData(); 
    }


  else if(state == LOAD){
    start_race = millis();
    load();
    digitalWrite(USV_DIS_PIN, HIGH);
    sendJson("loaded", 1); 
    state = ACCELERATION;
  }

  else if(state == ACCELERATION){
    
    if((set_speed - cur_speed) < 50){
      digitalWrite(USV_DIS_PIN, LOW); 
      state = DRIVE;
    }
    readData(); 
  }

  else if(state == DRIVE){
    readData();
  }

  else if(state == APPROACHSTOP){
     readData();
  }
  else if(state == FINISH){
    readData();
  }
  else if(state == TEST){
     #ifdef TEST_
      readData(); 
    #endif

  }

}

