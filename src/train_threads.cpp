#include <Thread.h>
#include <ThreadController.h>
#include <TimerOne.h>
#include <header.h>


ThreadController controll = ThreadController();

Thread threadReadData = Thread();
Thread threadProgramm = Thread();
Thread threadCheckTime = Thread();
Thread threadSendSpeed = Thread();

void thread_init(){

	// Configure the Threads

	//threadReadData.onRun(thReadData);
	//threadReadData.setInterval(1);

	threadProgramm.onRun(thProgramm);
	threadProgramm.setInterval(20);
	
	threadCheckTime.onRun(thCheckTime);
	threadCheckTime.setInterval(250);

	threadSendSpeed.onRun(thSendSpeed);
	threadSendSpeed.setInterval(100);

	// Adds to the controll
	controll.add(&threadCheckTime);
	controll.add(&threadProgramm);
	controll.add(&threadSendSpeed);
	//controll.add(&threadReadData);
	
	threadSendSpeed.enabled = false;
	threadCheckTime.enabled = false;
}

void thReadData()
{
	readData();
}

void thCheckTime()
{
	checkTime();
}

void thSendSpeed()
{
	uint16_t way = rot_count * (WHEEL_CIRC/ANZAHL_MAGNETE) / 100; // in cm
	//sendJson("speed", (int)cur_speed);	// mm/s
	Serial.println();
	sendJson("way", way);	// in cm
	Serial.println();
}

/********************************************************
 * Hauptprogrammablauf
 * 
 * 
 ********************************************************/
void thProgramm(){

	if(state == WAIT){

		if(!check_init_lastmotor) {initLastMotor();}
		start_race = millis();
		start_acc = millis();
		
		#ifdef RASPI_COMMUNICATION_OFF
			delay(1000);
			state = LOAD;
		#endif
		//unsigned long wait_start = millis();
		//while(WAIT_WHILE(wait_start, 6000)){yield();}
	
  	}


  	else if(state == LOAD){
		start_race = millis();
		load();
		//digitalWrite(USV_DIS_PIN, HIGH);
		sendJson("loaded", 1); 
		state = ACCELERATION;
		start_acc = millis();
		threadSendSpeed.enabled = true;
		threadCheckTime.enabled = false;
  	}

  	else if(state == ACCELERATION)
		{
			// while(WAIT_WHILE(start_acc, 3000));
			//digitalWrite(USV_DIS_PIN, LOW); 
			#ifdef RASPI_COMMUNICATION_OFF
				beschleunigen(50);
			#endif
			state = DRIVE;
  	}

  else if(state == DRIVE){
    
  }

  else if(state == APPROACHSTOP){
     
    //while(getLastStep() == 0 && state == APPROACHSTOP){yield();}
		last_step = get_distanz();
		if(last_step > 10)
		{
			state = STOPPING;
			sendJson("STOPPING", last_step);
		}
	 
  }
  else if(state == STOPPING){
    
  }
  else if(state == FINISH){
    if(threadSendSpeed.enabled){threadSendSpeed.enabled = false;} 
	if(threadCheckTime.enabled){threadCheckTime.enabled = false;}
  }
}



