#include <Arduino.h>
#include <ArduinoJson.h>
#include <Thread.h>
#include <ThreadController.h>
#include <header.h>



void setup() {
  Serial.begin(9600);

  pinModeSetup();

  PIDOutputLimit();

  initSetupVariables();

  setLEDstate();

  thread_init();

  sendJson("setup", 1);    // Test

}

void loop() {
  
  controll.run();
  readData();



}


/*
void loop() {

  if(state == WAIT){
    if(!check_init_lastmotor) {initLastMotor();}
    readData(); 
    #ifdef RASPI_COMMUNICATION_OFF
      delay(1000);
      state = LOAD;
    #endif
  }


  else if(state == LOAD){
    start_race = millis();
    start_acc = millis();
    load();
    //digitalWrite(USV_DIS_PIN, HIGH);
    sendJson("loaded", 1); 
    state = ACCELERATION;
    start_acc = millis();
  }

  else if(state == ACCELERATION){
   // while(WAIT_WHILE(start_acc, 3000));
      //digitalWrite(USV_DIS_PIN, LOW); 
      #ifdef RASPI_COMMUNICATION_OFF
        beschleunigen(50);
      #endif
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

}
*/
