#include <Wire/Wire.h>
#include <I2Cdev/I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>

#define		PIN_INT	2

// mpu control/status vars
bool		dmpReady = false;
uint8_t		mpuIntStatus;
uint8_t		devStatus;
uint16_t	packetSize;
uint16_t	fifoCount;
uint8_t		fifoBuffer[64];

Quaternion	q;				// [w, x, y, z] quaternion container
VectorFloat	gravity;
float		euler[3];
float		ypr[3];

volatile bool mpuInterrupt = false;

MPU6050	mpu;

void dmpDataReady() {
	mpuInterrupt = true;
}

void initGyro() {

	Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();

	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful.") : F("MPU6050 connection fail."));

	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

	if (devStatus == 0) {
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(F("Enabling interrupt detection (Arduino external interrupt."));
		attachInterrupt(digitalPinToInterrupt(PIN_INT), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();

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

float getPitch() {
	return ypr[1];
}

// the loop function runs over and over again until power down or reset
void readGyroData() {
	if (!dmpReady)
		return;

	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();

	fifoCount = mpu.getFIFOCount();

	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));
	}
	else if (mpuIntStatus & 0x02) {
		//wai for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize)
			fifoCount = mpu.getFIFOCount();

		mpu.getFIFOBytes(fifoBuffer, packetSize);

		fifoCount -= packetSize;

		//mpu.dmpGetQuaternion(&q, fifoBuffer);
		//Serial.print("quat\t");
		//Serial.print(q.w);
		//Serial.print("\t");
		//Serial.print(q.x);
		//Serial.print("\t");
		//Serial.print(q.y);
		//Serial.print("\t");
		//Serial.print(q.z);
		//Serial.print("\t");

		//mpu.dmpGetQuaternion(&q, fifoBuffer);
		//mpu.dmpGetEuler(euler, &q);
		//Serial.print("euler\t");
		//Serial.print(euler[0] * 180 / M_PI);
		//Serial.print("\t");
		//Serial.print(euler[1] * 180 / M_PI);
		//Serial.print("\t");
		//Serial.print(euler[2] * 180 / M_PI);
		//Serial.print("\t");

		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		Serial.print("ypr\t");
		Serial.print(ypr[0] * 180 / M_PI);
		Serial.print("\t");
		Serial.print(ypr[1] * 180 / M_PI);
		Serial.print("\t");
		Serial.println(ypr[2] * 180 / M_PI);
	}
}

