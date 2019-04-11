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

    handleData(action, payload);
    
    #ifdef TEST_
      handleTestData(action, payload);
    #endif

    // Print values.
    /*
    Serial.println(sender);
    Serial.println(action);
    Serial.println(payload);
    */


}

void handleData(const char* action, int payload){
  
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

#ifdef TEST_
void handleTestData(const char* action, int payload){
    
    if(strcmp(action, "LOAD") == 0)
    {
          load();
    }
    else if(strcmp(action, "MOTLAST") == 0)
    {
          if(payload == 0){
            digitalWrite(MOT_LAST_PIN, LOW);
            Serial.println("Stop Motor");
          }
          else if(payload < 0)
          {
            initLastMotor();
            Serial.println("INIT MOT");
          }
          else
          {
            digitalWrite(MOT_LAST_PIN, HIGH);
            Serial.println("Start Motor");
          } 
    }
    else if(strcmp(action, "HALL") == 0)
    {
      int hall_nbr_pin = payload/10;
      int task = payload - hall_nbr_pin;
      if(hall_nbr_pin == 1){ hall_nbr_pin = HALL_LAST_1_PIN; }
      else if(hall_nbr_pin == 2){ hall_nbr_pin = HALL_LAST_2_PIN; }

      switch (task)
      {
        case 1:
          Serial.print("Hallsensor Read: ");
          Serial.println(digitalRead(hall_nbr_pin)); 
          break;
        case 2:
          long hall_start = millis();
          while(!digitalRead(hall_nbr_pin) && (millis() - hall_start < 6000))
          Serial.print("Time (ms): ");
          Serial.println((millis() - hall_start));
          break;

        default:
          break;
      }
    }
    else if(strcmp(action, "ENDSCHALTER") == 0)
    {
      long ends_start = millis();
      while(!digitalRead(ENDSCHALTER_PIN) && WAIT_WHILE(ends_start, 6000))
      Serial.print("Time (ms): ");
      Serial.println((millis() - ends_start));
    }
}
#endif

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

