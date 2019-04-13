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

