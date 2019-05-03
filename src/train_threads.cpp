#include <Thread.h>
#include <ThreadController.h>
#include <TimerOne.h>
#include <header.h>


ThreadController controll = ThreadController();
ThreadController timer_control = ThreadController();


Thread threadReadData = Thread();
Thread threadProgramm = Thread();
Thread threadCheckTime = Thread();


void thReadData()
{
	readData();
}

void thProgramm(){

	if(state == WAIT){
		if(!check_init_lastmotor) {initLastMotor();}
		#ifdef RASPI_COMMUNICATION_OFF
		delay(1000);
		state = LOAD;
		#endif
  	}


  	else if(state == LOAD){
		start_race = millis();
		start_acc = millis();
		load();
		//digitalWrite(USV_DIS_PIN, HIGH);
		sendJson("loaded", 1); 
		state = ACCELERATION;
		start_acc = millis();
  	}

  	else if(state == ACCELERATION){
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
     
     while(getLastStep() == 0 && state == APPROACHSTOP);
	 if(state == APPROACHSTOP)
	 {
		last_step = get_distanz();
		state = STOPPING;
		sendJson("STOPPING", last_step);
	 }
  }
  else if(state == STOPPING){
    
  }
  else if(state == FINISH){
    
  }
}

void thCheckTime()
{
	checkTime();
}

void timerCallback(){
	timer_control.run();
}

void thread_init(){

	//threadReadData.onRun(thReadData);
	//threadReadData.setInterval(20);

	// Configure hisThread
	threadProgramm.onRun(thProgramm);
	threadProgramm.setInterval(20);
	
	threadCheckTime.onRun(thCheckTime);
	threadCheckTime.setInterval(250);

	// Adds to the controll
	//controll.add(&threadReadData);
	controll.add(&threadCheckTime);
	controll.add(&threadProgramm);
//	timer_control.add(&threadReadData);
/*
	Timer1.initialize(50000);
	Timer1.attachInterrupt(timerCallback);
	Timer1.start();
	*/
}



