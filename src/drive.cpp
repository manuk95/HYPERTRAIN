#include <Arduino.h>
#include <header.h>
#include <PID_v1.h>

/********************************************************
 * DRIVE
 * Zwei drive-modi:     MODE_PID    (Regelung)
 *                      MODE_NORMAL (Steuerung)
 *
 * Wird PWM_OUTPUT_LIMITS_ definiert, wird die Motor-Spannung auf MAX_MOTOR_U begrenzt.
 * 
 ********************************************************/

//Specify the links and initial tuning parameters
int maxPWMoutput;
double Kp=10, Ki=5, Kd=0;
PID myPID(&cur_speed, &Output, &set_speed, Kp, Ki, Kd, DIRECT);

void PIDsetup()
{
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  PIDOutputLimit();
}


void PIDupdate()
{
  myPID.Compute();
  analogWrite(HBRI_F_PIN, Output); //PWM Output 0=always off, 255= always on
}

void PIDOutputLimit()
{
  maxPWMoutput = 255;
  #ifdef PWM_OUTPUT_LIMITS_
    maxPWMoutput = (int) 255 * MAX_MOTOR_U / ((analogRead(U_GLEIS_PIN) * 68) / 12);
  #endif
  myPID.SetOutputLimits((double) 0, (double) maxPWMoutput);
}

void beschleunigen(int speed)
{
  if(speed >= 0)  { digitalWrite(LED_VORNE_PIN, HIGH); digitalWrite(LED_HINTEN_PIN, LOW); } 
  else            { digitalWrite(LED_HINTEN_PIN, HIGH); digitalWrite(LED_VORNE_PIN, LOW); }

  if(acc_modus == MODE_PID)
  {
      #ifdef DEBUG_
          Serial.print("MODE_PID: Start Beschleunigung auf ");
          Serial.print(speed);
          Serial.println(" mm/s");
      #endif
    set_speed = speed/255 * MAX_SPEED;
    start_isr = millis();
    PIDupdate();
  }
  else if(acc_modus == MODE_NORMAL)
  {
      #ifdef DEBUG_
          Serial.print("MODE_NORMAL: Start Beschleunigung von ");
          Serial.print(Output);
          Serial.print(" auf ");
          Serial.print(speed);
          Serial.println("! MAX: 255 (oder maxPWMoutput)");
      #endif
      set_speed = speed;

    if(speed > Output)
    {
      for(int i = (int) Output; (i < speed) && (i <= maxPWMoutput); i++)
      {
        PWMoutput(i+1);
      }
      Output = speed;
    }
    else
    {
      for(int i = (int) Output; (i > speed) && (i <= maxPWMoutput); i--)
      {
        PWMoutput(i-1);
      }
      Output = speed;
    }
     #ifdef DEBUG_
          Serial.println("Beschleunigung fertig!");
      #endif
  }
}

void PWMoutput(int output){
  if(output == 0){
    analogWrite(HBRI_F_PIN, output); 
    analogWrite(HBRI_R_PIN, output); 
    delay(10);
  }
  else if(output > 0){
    analogWrite(HBRI_F_PIN, output); 
    if(output < 50) { delay(10); }
    else { delay(13); } 
  }
  else
  {
    output = output * (-1);
    analogWrite(HBRI_R_PIN, output); 
    delay(10);
  }
  #ifdef DEBUG_2
          Serial.print("PWMOut: ");
          Serial.println(output);
  #endif
  

}