#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>


/*              Variabeln               */
  // Variabeln Zeichen einlesen 
  char command[70]; // beinhaltet den aktuellen befehl
  int readNumber=0;  



/* Die JSON-Daten werden versendet.   */
void sendJson(const char* action, const char* payload) {

  StaticJsonDocument<200> doc;

  doc["sender"] = "arduino";
  doc["action"] = action;
  doc["payload"] = payload;

  serializeJson(doc, Serial);
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

}


/*                                      */
/* Die JSON-Daten werden ausgewertet.   */
/*                                      */
void parsJSON(char input[70]) {
    // Initialize serial port

    while (!Serial) continue;
    
    StaticJsonDocument<70> doc;


    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }


    const char* sender = doc["sender"];
    const char* action = doc["action"];
    int payload = doc["payload"].as<int>();


    // Print values.
    Serial.println(sender);
    Serial.println(action);
    Serial.println(payload);


    if(strcmp(action, "accelerate") == 0){
      if(payload < 0 || payload > 100) {sendJSONAccError();}
      else {cur_speed = (payload / 100) * MAX_SPEED;}
    }
    else if(strcmp(action, "approachstop") == 0)
    {
      state = APPROACHSTOP;
        #ifdef DEBUG_
          Serial.println("Langsames Anhalten");
        #endif

    }
    else if(strcmp(action, "TEST") == 0)
    {
      #ifdef DEBUG_
          Serial.println("TEST");
      #endif
      state = TEST;
    }


}

/*                                                              */
/* Zeichen werden eingelesen und an den Parser weitergegeben.   */
/*                                                              */
void readData(){
  if (Serial.available() > 0) {
      int input = 0;
      input = Serial.read();
      char currentChar = input;
      if(currentChar  == '}') 
      {
        command[readNumber] =currentChar;
        parsJSON(command);
        readNumber =0;
      } else
      {
        command[readNumber] = currentChar ; 
        readNumber++;
      }
  }
}

/*                                                                                               */
/* Auf ACCERLERATION wird eine -1 gesendet, weil eine ungültige Geschwindigkeit erkannt wurde.   */
/* Wird auch als JSON-Beispiel verwendet.                                                       */
void sendJSONAccError(){
     String output = "accelerate";
      char action[15];
      char payload[15];
      output.toCharArray(action, 15);
      output = "-1";
      output.toCharArray(payload, 15);

      sendJson(action, payload);
}

