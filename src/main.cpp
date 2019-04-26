#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>



void setup() {
  Serial.begin(9600);

  pinModeSetup();

  PIDOutputLimit();

  initSetupVariables();

  digitalWrite(LED_BUILTIN, HIGH); 
  
  sendJson("setup", 1);    // Test
}



void loop() {

  if(state == WAIT){
    readData(); 
  }


  else if(state == LOAD){
    start_race = millis();
    if(!check_init_lastmotor) {initLastMotor();}
    load();
    digitalWrite(USV_DIS_PIN, HIGH);
    sendJson("loaded", 1); 
    state = ACCELERATION;
    start_acc = millis();
  }

  else if(state == ACCELERATION){
    while(WAIT_WHILE(start_acc, 3000));
      digitalWrite(USV_DIS_PIN, LOW); 
      state = DRIVE;
  }

  else if(state == DRIVE){
    readData();
  }

  else if(state == APPROACHSTOP){
     readData();
     last_step = get_distanz();
     state = STOPPING;
  }
  else if(state == STOPPING){
    readData();
  }
  else if(state == FINISH){
    readData();
    if(!check_init_lastmotor){initLastMotor();}
  }
  checkTime();
  get_distanz();

}

