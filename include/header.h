#ifndef HEADER_H
#define HEADER_H

#include <Thread.h>
#include <ThreadController.h>


// Configure DEBUG- / TEST- LEVEL & FUNCTION
//#define DEBUG_
//#define DEBUG_2
#define TEST_
//#define TEST_ROT    
// #define PWM_OUTPUT_LIMITS_
#define PWM_LOAD_MOTOR
//#define RASPI_COMMUNICATION_OFF

// Eingänge
#define IR_PIN A0
#define U_GLEIS_PIN A7
#define TACHO_PIN 2
#define HALL_LAST_1_PIN A2
#define HALL_LAST_2_PIN A1
#define ENDSCHALTER_PIN A3


// Ausgänge
#define HBRI_F_PIN 9
#define HBRI_R_PIN 10
#define MOT_LAST_PIN 11
#define USV_DIS_PIN 12
#define LED_VORNE_PIN 5
#define LED_HINTEN_PIN 6

#define WHEEL_CIRC 942 //x 0.1mm
#define ANZAHL_MAGNETE 4
#define MAX_STRECKE 230000 //(23 * 10000) // in 0.1mm, zwei Runden
#define MAX_ROT_COUNT (MAX_STRECKE / (WHEEL_CIRC / ANZAHL_MAGNETE)) 

#define MAX_SPEED 300 // cm/s
#define MAX_MOTOR_U 15
#define WAIT_WHILE(start_var, max_zeit) (millis() - start_var < max_zeit)

/*              Variabelen              */
extern int rot_count;
extern long start_isr;
extern long start_race;
extern long start_acc;
extern bool check_init_lastmotor;
extern int last_step;

/*            PID Variabeln         */
extern double set_speed, cur_speed, Output;

/*              Typen              */
enum state_t {
  WAIT,
  LOAD,
  ACCELERATION,
  DRIVE,
  APPROACHSTOP,
  STOPPING,
  FINISH
};

enum acc_modus_t {
  MODE_PID,
  MODE_NORMAL
};

enum drive_modus_t {
  MODE_NORM,
  MODE_WAY
};

/*            STATE-MACHINE         */
extern state_t state;
extern acc_modus_t acc_modus;
extern drive_modus_t drive_modus;

/*                                      */
/*              Funktionen              */
/*                                      */

//variables 
void initSetupVariables();
void pinModeSetup();
void setLEDstate();


// function
void isr_tacho_count();
uint16_t get_distanz();
uint16_t getLastStep();
double radiant();
void get_speed();
void checkTime();


// JSON
void readData();
void sendJson(String sAction, int iPayload);
void parsJSON(char input[]);
void handleData(const char* action, int payload);
void handleTestData(const char* action, int payload);

// DRIVE
void PIDupdate();
void PIDsetup();
void PIDOutputLimit();

void beschleunigen(int speed);
void PWMoutput(int output);

// LOADING
void load();
void initLastMotor();


/*                                      */
/*              THREADS                 */
/*                                      */
extern ThreadController controll;

extern Thread threadReadData;
extern Thread threadProgramm;
extern Thread threadCheckTime;
extern Thread threadSendSpeed;

void thread_init();
void thSendSpeed();
void thProgramm();
void thCheckTime();

#endif