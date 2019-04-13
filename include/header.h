#ifndef HEADER_H
#define HEADER_H

#define DEBUG_
#define TEST_
// #define PWM_OUTPUT_LIMITS_

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



#define WHEEL_CIRC 300 //x 0.1mm
#define MAX_STRECKE 1700 // in cm, zwei Runden
#define MAX_ROT_COUNT 567 // MAX_STRECKE / (cm) WHEEL_CIRC = 567

#define IR_WINKEL 20 // in GRAD

#define MAX_SPEED 300 // cm/s
#define MAX_MOTOR_U 15
#define WAIT_WHILE(start_var, max_zeit) (millis() - start_var < max_zeit)




/*              Variabelen              */
extern int rot_count;
extern long start_isr;
extern long start_race;


/*            PID Variabeln         */
extern double set_speed, cur_speed, Output;



/*              Typen              */
enum state_t {
  WAIT,
  LOAD,
  ACCELERATION,
  DRIVE,
  APPROACHSTOP,
  FINISH,
  TEST
};

enum modus_t {
  MODE_PID,
  MODE_NORMAL
};

/*            STATE-MACHINE         */
extern state_t state;
extern modus_t modus;

/*                                      */
/*              Funktionen              */
/*                                      */

//variables 
void initSetupVariables();
void pinModeSetup();


// function
void isr_tacho_count();
uint16_t get_distanz();
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



#endif