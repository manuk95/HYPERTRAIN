#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>



void setup() {
  Serial.begin(9600);

  pinModeSetup();

  PIDOutputLimit();

  initSetupVariables();

  setLEDstate();

  sendJson("setup", 1);    // Test

}



void loop() {

  if(state == WAIT){
    if(!check_init_lastmotor) {initLastMotor();}
    readData(); 
  }


  else if(state == LOAD){
    start_race = millis();
    load();
    //digitalWrite(USV_DIS_PIN, HIGH);
    sendJson("loaded", 1); 
    state = ACCELERATION;
    start_acc = millis();
  }

  else if(state == ACCELERATION){
   // while(WAIT_WHILE(start_acc, 3000));
      //digitalWrite(USV_DIS_PIN, LOW); 
      state = DRIVE;
  }

  else if(state == DRIVE){
    readData();
  }

  else if(state == APPROACHSTOP){
     readData();
     while(getLastStep() == 0);
     last_step = get_distanz();
     state = STOPPING;
     sendJson("STOPPING", last_step);
  }
  else if(state == STOPPING){
    readData();
  }
  else if(state == FINISH){
    readData();
  }
  checkTime();
  get_distanz();

}

