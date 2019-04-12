#include <Arduino.h>
#include <ArduinoJson.h>
#include <header.h>


/*              Variabeln               */
  // Variabeln Zeichen einlesen 
  char command[70]; // beinhaltet den aktuellen befehl
  int readNumber=0;  



/* Die JSON-Daten werden versendet.   */
void sendJson(String sAction, int iPayload){
      /*
      char cAction[15];
      char cPayload[15];
      sAction.toCharArray(cAction, 15);
      itoa(iPayload, cPayload, 10);
      */
      StaticJsonDocument<200> doc;

      doc["sender"] = "arduino";
      doc["action"] = sAction;
      doc["payload"] = iPayload;

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


    // const char* sender = doc["sender"];
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
      if(payload < 0 || payload > 100) {sendJson("accelerate", -1);}
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
    else if(strcmp(action, "DRIVE") == 0)
    {
        if(payload == 0){
          digitalWrite(HBRI_F_PIN, LOW);
          delay(200);
          digitalWrite(HBRI_R_PIN, LOW); 
        }
        else if(payload > 0){
          digitalWrite(HBRI_R_PIN, LOW); 
          delay(200);
          digitalWrite(HBRI_F_PIN, HIGH); 
        }
        else
        {
          digitalWrite(HBRI_F_PIN, LOW); 
          delay(200);
          digitalWrite(HBRI_R_PIN, HIGH); 
        }
    }
    else if(strcmp(action, "ACC") == 0)
    {
      Serial.println("accelerate");
      PWMoutput(payload);
    }
    else if (strcmp(action, "beschl") == 0) {
      Serial.print("beschleunigung: ");
      Serial.println(payload);
      beschleunigen(payload);
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
      int task = payload - hall_nbr_pin*10;
      if(hall_nbr_pin == 1)     { hall_nbr_pin = HALL_LAST_1_PIN; }
      else if(hall_nbr_pin == 2){ hall_nbr_pin = HALL_LAST_2_PIN; }

      #ifdef DEBUG_
          Serial.print("hallsensor_nmbr: ");
          Serial.print(hall_nbr_pin);
          Serial.print(" task: ");
          Serial.print(task);
          Serial.println();
      #endif

      switch (task)
      {
        case 1:
          Serial.print("Hallsensor Read: ");
          Serial.println(digitalRead(hall_nbr_pin)); 
          break;
        case 2:
          long hall_start = millis();
          while(digitalRead(hall_nbr_pin) && WAIT_WHILE(hall_start, 6000));
          Serial.print("Time (ms): ");
          Serial.println((millis() - hall_start));
          break;
        
        /*
        default:
          Serial.print("Hallsensor Anweisung ungültig");
          break; 
        */
      }
    }
    else if(strcmp(action, "ENDSCHALTER") == 0)
    {
      switch (payload)
      {
        case 1:
          Serial.print("Endschalter Read: ");
          Serial.println(digitalRead(ENDSCHALTER_PIN)); 
          break;
        case 2:
          long ends_start = millis();
          while(digitalRead(ENDSCHALTER_PIN) && WAIT_WHILE(ends_start, 6000));
          Serial.print("Time (ms): ");
          Serial.println((millis() - ends_start));
      }
      
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


