#include <Arduino.h>
#include <header.h>
#include <PID_v1.h>

int rot_count;
long start_isr;
long start_race;
float dauer;

double set_speed, cur_speed, Output;

int maxPWMoutput;

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  SHARP IR Sensor  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_gp2d12 () {
  uint16_t value = analogRead (IR_PIN);
  if (value < 10) value = 10;
  return ((67870.0 / (value - 3.0)) - 40.0); // Distanz in mm
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  COUNT  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void isr_tacho_count(){
  rot_count++;
  meas_time();
  
  if(modus == MODE_PID) {PIDupdate();}
  if(rot_count >= MAX_ROT_COUNT)
  {
    rot_count = 0;
    if(state == DRIVE || state == ACCELERATION) {state = APPROACHSTOP;}
  }
}



/* *** *** *** *** *** *** *** *** *** *** *** *** *** ***  MEAS SPEED  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
void meas_time(){
  dauer = millis() - start_isr;
  start_isr = millis();
  cur_speed = (double) (WHEEL_CIRC/10) / (dauer /1000);           // mm/s
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** DISTANZ  *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
uint16_t get_distanz()
{
  uint16_t range = get_gp2d12();
  uint16_t distanz = range * cos(radiant());
  if(distanz < 100)
  {
    return distanz;
  }
  distanz = 0;
  return distanz;
}

double radiant()
{
  return (double) (IR_WINKEL / 360 * 2 * 3.14);
}


/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/

//Specify the links and initial tuning parameters
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
 // Input = analogRead(PIN_INPUT);
  myPID.Compute();
  analogWrite(HBRI_F_PIN, Output); //PWM Output 0=always off, 255= always on
}

void PIDOutputLimit()
{
  
  maxPWMoutput = (int) 255 * MAX_MOTOR_U / ((analogRead(U_GLEIS_PIN) * 68) / 12);
  myPID.SetOutputLimits((double) 0, (double) maxPWMoutput);
}

void beschleunigen(int speed)
{
  if(modus == MODE_PID)
  {
    set_speed = speed;
    start_isr = millis();
    PIDupdate();
  }
  else if(modus == MODE_NORMAL)
  {
    if(speed > Output)
    {
      for(int i = (int) Output; (i < speed) && (i < maxPWMoutput); i++)
      {
        PWMoutput(i);
      }
    }
    else
    {
      for(int i = (int) Output; (i > speed) && (i < maxPWMoutput); i--)
      {
        PWMoutput(i);
      }
    }
  }
}

void PWMoutput(int output){
  if(output == 0){
    analogWrite(HBRI_F_PIN, output); 
    analogWrite(HBRI_R_PIN, output); 
    delay(50);
  }
  else if(output > 0){
    analogWrite(HBRI_F_PIN, output); 
    delay(50);
  }
  else
  {
    analogWrite(HBRI_R_PIN, output); 
    delay(50);
  }
  

}

/********************************************************
 * LAST AUFLADEN
 * initLastMotor bringt den Aufnahmemechanismus in die richtige Stellung.
 * 
 * Wenn der Endschalter betätigt wird, start_isret der Motor.
 * Nach zwei gemessenen Umdrehungen ist die Last aufgeladen
 * und der Motor schaltet aus. 
 ********************************************************/

void load(){
  beschleunigen(25);
  
  

  while(!digitalRead(ENDSCHALTER_PIN))
  beschleunigen(0);

  digitalWrite(MOT_LAST_PIN, HIGH);
  
  while(!digitalRead(HALL_LAST_2_PIN));

  while(digitalRead(HALL_LAST_2_PIN));

  while(!digitalRead(HALL_LAST_2_PIN));

  digitalWrite(MOT_LAST_PIN, LOW);
  
}

void initLastMotor(){
  
  digitalWrite(MOT_LAST_PIN, HIGH);
  while(!digitalRead(HALL_LAST_1_PIN));
  digitalWrite(MOT_LAST_PIN, LOW);

}

/********************************************************
 * CHECK TIME
 * 
 ********************************************************/
void checkTime()
{
  if((millis() - start_race) > 130000){
    if(state < 3){
      state = APPROACHSTOP;
    }
  }
  else if((millis() - start_race) > 240000)
  {
    beschleunigen(0);
    state = FINISH;
  }
}