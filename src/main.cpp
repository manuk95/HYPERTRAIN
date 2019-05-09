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
  sendJson("setup", 1);    // Restart
}

void loop() {
  
  controll.run();
  readData();

}