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


void handleData(const char* action, int payload){
  
    if(strcmp(action, "start") == 0){
      state = LOAD;
      start_race = millis();
		  start_acc = millis();
    }
    else if(strcmp(action, "stop") == 0){
      beschleunigen(0);
      if(state == FINISH) { state = WAIT; 
                            start_race = millis();
		                        start_acc = millis(); 
                            rot_count = 0;}
      else                { state = FINISH; 
                           start_race = millis();
		                        start_acc = millis(); 
                            rot_count = 0;}
      #ifdef DEBUG_
        Serial.println("## stop");
      #endif
    }
    else if(strcmp(action, "accelerate") == 0){
      if(state == DRIVE || state == ACCELERATION)
      {
        if(payload < 0 || payload > 100) {sendJson("accelerate", -1);}
        else {beschleunigen(255 * payload / 100);}
      }
    }
    else if(strcmp(action, "approachstop") == 0)
    {
      if(state == DRIVE || state == ACCELERATION) {state = APPROACHSTOP;}
        #ifdef DEBUG_
          Serial.println("Langsames Anhalten");
        #endif
    }

}

#ifdef TEST_
void handleTestData(const char* action, int payload){
    
    if(strcmp(action, "LOAD") == 0)
    {
          load();
    }
    else if(strcmp(action, "STOP") == 0){
      beschleunigen(0);
    }
    else if(strcmp(action, "DRIVE") == 0)
    {
        
    }
    else if(strcmp(action, "ACC") == 0)
    {
      #ifdef DEBUG_
        Serial.println("accelerate");
      #endif
      PWMoutput(payload);
    }
    else if (strcmp(action, "beschl") == 0) {
      #ifdef DEBUG_
        Serial.print("beschleunigung: ");
        Serial.println(payload);
      #endif
      beschleunigen(payload);
    }
    
    else if(strcmp(action, "MOTLAST") == 0)
    {
          if(payload == 0){
            digitalWrite(MOT_LAST_PIN, LOW);
            #ifdef DEBUG_
              Serial.println("Stop Motor");
            #endif
          }
          else if(payload < 0)
          {
            initLastMotor();
            #ifdef DEBUG_
              Serial.println("INIT MOT");
            #endif
          }
          else
          {
            digitalWrite(MOT_LAST_PIN, HIGH);
            #ifdef DEBUG_
              Serial.println("Start Motor");
            #endif
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
          #ifdef DEBUG_
            Serial.print("Hallsensor Read: ");
          #endif
          Serial.println(digitalRead(hall_nbr_pin)); 
          break;
        case 2:
          long hall_start = millis();
          while(digitalRead(hall_nbr_pin) && WAIT_WHILE(hall_start, 6000));
          #ifdef DEBUG_
            Serial.print("Time (ms): ");
            Serial.println((millis() - hall_start));
          #endif
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
      Serial.println(input);
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

/*                                                              */
/* Zeichen werden eingelesen und an den Parser weitergegeben.   */
/*                                                              */
void readData(){
   #ifdef DEBUG_2
        Serial.println(" -");
       Serial.println(millis());
    #endif

  if (Serial.available() > 0) {
      int input = 0;
      input = Serial.read();
      char currentChar = input;
      if(currentChar  == '}') 
      {
        command[readNumber] =currentChar;
        parsJSON(command);
        readNumber =0;
      } 

      else if(currentChar == '{')
      {
       readNumber =0; 
       command[readNumber] =currentChar;
       readNumber++;
      }

      else
      {
        command[readNumber] = currentChar ; 
        readNumber++;
      }
  }
}


