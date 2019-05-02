#include <Thread.h>
#include <ThreadController.h>
#include <header.h>

/*
// ThreadController that will controll all threads
ThreadController controll = ThreadController();

//My Thread
Thread threadReadData = Thread();
//His Thread
Thread threadBeschleunigen = Thread();
//Blink Led Thread
Thread threadProgramm = Thread();


// callback for threadBeschleunigen	
void thBeschleunigen()
{
  if(modus == MODE_NORMAL)
  {
		set_speed;

		if(set_speed > Output)
		{
		for(int i = (int) Output; (i < set_speed) && (i < maxPWMoutput); i++)
		{
			PWMoutput(i+1);
		}
		Output = set_speed;
		}
		else
		{
		for(int i = (int) Output; (i > set_speed) && (i < maxPWMoutput); i--)
		{
			PWMoutput(i-1);
		}
		Output = set_speed;
		}

  }

  if(set_speed >= 0)  { digitalWrite(LED_VORNE_PIN, HIGH); digitalWrite(LED_HINTEN_PIN, LOW); } 
  else            { digitalWrite(LED_HINTEN_PIN, HIGH); digitalWrite(LED_VORNE_PIN, LOW); }
}


}

// callback for hisThread
void boringCallback(){
	Serial.println("BORING...");
}

// callback for blinkLedThread
void blinkLed(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("blinking: ");
	Serial.println(ledStatus);
}

void setup(){
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);

	// Configure myThread
	myThread.onRun(niceCallback);
	myThread.setInterval(500);

	// Configure hisThread
	hisThread.onRun(boringCallback);
	hisThread.setInterval(250);
	
	// Configure blinkLedThread
	blinkLedThread.onRun(blinkLed);
	blinkLedThread.setInterval(100);

	// Adds myThread to the controll
	controll.add(&myThread);

	// Adds hisThread and blinkLedThread to groupOfThreads
	groupOfThreads.add(&hisThread);
	groupOfThreads.add(&blinkLedThread);

	// Add groupOfThreads to controll
	controll.add(&groupOfThreads);
	
}

void loop(){
	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
	controll.run();

	// Rest of code
	float h = 3.1415;
	h/=2;
}

*/