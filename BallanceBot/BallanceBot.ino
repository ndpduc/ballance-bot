/*
 Name:		BalanceBot.ino
 Created:	12/4/2015 8:51:46 PM
 Author:	ndpdu
*/



// the setup function runs once when you press reset or power the board
#include <PID/PID_v1.h>
#include <TaskScheduler/src/TaskScheduler.h>
						

// mpu control/status vars
#define	KP		250
#define	KI		20
#define	KD		0

double	SetPoint	= 90;
double	pitch		= 90;
double	input		= 0;

PID			pid(&pitch, &input, &SetPoint, KP, KI, KD, REVERSE);
Scheduler	scheduler;

Task		ballanceBotTask(50, -1, loopBallanceBot);

void setup() {
	Serial.begin(115200);

	initGyro();
	initMotors();

	pid.SetMode(AUTOMATIC);
	pid.SetSampleTime(50);
	pid.SetOutputLimits(-255., 255.);

	Serial.println("Enable Ballance Bot Task...");
	ballanceBotTask.enable();

	Serial.println("Initialize Scheduler...");
	scheduler.init();
	
	Serial.println("Add Ballance Bot Task.");
	scheduler.addTask(ballanceBotTask);
}

// the loop function runs over and over again until power down or reset
void loop() {
	scheduler.execute();
	pid.Compute();

	readGyroData();
}

//do this loop 10ms/time
void loopBallanceBot() {
	pitch = getPitch();

	Serial.print("Input Value: ");
	Serial.println(input);

	go(input > 0 ? 0 : 1, abs(input));
}
