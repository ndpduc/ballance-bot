/*
 Name:		BalanceBot.ino
 Created:	12/4/2015 8:51:46 PM
 Author:	ndpdu
*/



// the setup function runs once when you press reset or power the board
#include <MPU6050_6Axis_MotionApps20.h>
#include <MPU6050.h>

#define OUTPUT_READABLE_YAWPITCHROLL

// mpu control/status vars
bool		dmpReady		= false;
uint8_t		mpuIntStatus;
uint8_t		devStatus;
uint16_t	packetSize;

volatile bool mpuInterrupt = false;

MPU6050	mpu;

void setup() {
	Serial.begin(115200);

	Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();

	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful.") : F("MPU6050 connection fail."));

	devStatus = mpu.dmpInitialize();

	if (devStatus == 0) {
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (!dmpReady)
		return;

	while(!mpuIn)
}
