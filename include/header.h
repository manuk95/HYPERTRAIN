#ifndef HEADER_H
#define HEADER_H

#define DEBUG_
#define TEST_

#define IR_PIN A0
#define U_GLEIS_PIN A7
#define TACHO_PIN 2
#define HBRI_F_PIN 9
#define HBRI_R_PIN 10
#define MOT_LAST_PIN 11
#define USV_DIS_PIN 12

#define HALL_LAST_1_PIN A2
#define HALL_LAST_2_PIN A1
#define ENDSCHALTER_PIN A3


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
extern float dauer;

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
bool checkSpeed();
void isr_tacho_count();
void sendJson(const char* action, const char* payload);
void parsJSON(char input[]);
void handleData(const char* action, int payload);
void handleTestData(const char* action, int payload);

uint16_t get_distanz();
double radiant();
void sendJSONAccError();
void readData();
void meas_time();


void PIDupdate();
void PIDsetup();
void PIDOutputLimit();

void beschleunigen(int speed);
void PWMoutput(int output);


void load();
void initLastMotor();

void checkTime();

#endif