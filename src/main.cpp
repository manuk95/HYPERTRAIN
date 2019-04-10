#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>

state_t state;
modus_t modus;

void setup() {
  Serial.begin(9600);
  pinMode (IR_PIN, INPUT);
  
  
  // pinMode(TACHO_PIN, INPUT_PULLUP);
  // pinMode(ENDSCHALTER_PIN, INPUT_PULLUP);

  pinMode(TACHO_PIN, INPUT);
  pinMode(ENDSCHALTER_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(TACHO_PIN), isr_tacho_count, RISING);
  rot_count = 0;
  
  
  
  
  sendJSONAccError();    // Test
  
  set_speed = 0;
  initLastMotor();
  modus = MODE_NORMAL;
  state = WAIT;
}



void loop() {

      
 
  if(state == WAIT){
    readData(); 
    }


  else if(state == LOAD){
    start_race = millis();
    load();
    digitalWrite(USV_DIS_PIN, HIGH);
    sendJson("loaded", "1"); 
    state = ACCELERATION;
  }

  else if(state == ACCELERATION){
    
    if((set_speed - cur_speed) < 20){
      digitalWrite(USV_DIS_PIN, LOW); 
      state = DRIVE;
    } else {
      PIDupdate();
    }

  }

  else if(state == DRIVE){
    readData();
  }

  else if(state == APPROACHSTOP){}
  else if(state == FINISH){}
  else if(state == TEST){
     #ifdef TEST_
      
    #endif

  }

}

