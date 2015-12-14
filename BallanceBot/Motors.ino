#define PIN_ENA		11
#define PIN_ENB		6
#define PIN_A1		9
#define PIN_A2		10
#define PIN_B1		8
#define PIN_B2		7

enum {
	MOTORA = 0,
	MOTORB,
};

enum {
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT,
};

void initMotors() {
	pinMode(PIN_ENA, OUTPUT);
	pinMode(PIN_ENB, OUTPUT);

	pinMode(PIN_A1, OUTPUT);
	pinMode(PIN_A2, OUTPUT);

	pinMode(PIN_B1, OUTPUT);
	pinMode(PIN_B2, OUTPUT);

	digitalWrite(PIN_A1, LOW);
	digitalWrite(PIN_A2, LOW);
	digitalWrite(PIN_B1, LOW);
	digitalWrite(PIN_B2, LOW);
}

void moveMotor(uint8_t motor, uint8_t dir, double input) {
	uint8_t pin1;
	uint8_t pin2;
	uint8_t pinE;

	uint8_t inputSpeed = (uint8_t)min(abs(input), 255); 

	if (motor == MOTORA) {
		pin1 = PIN_A1;
		pin2 = PIN_A2;
		pinE = PIN_ENA;
	}
	else if (motor == MOTORB) {
		pin1 = PIN_B1;
		pin2 = PIN_B2;
		pinE = PIN_ENB;
	}

	if (dir == FORWARD) {
		digitalWrite(pin1, HIGH);
		digitalWrite(pin2, LOW);
	}
	else if (dir == BACKWARD) {
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, HIGH);
	}

	analogWrite(pinE, input);
}

void stopMotor(uint8_t motor) {
	uint8_t pin1;
	uint8_t pin2;
	uint8_t pinE;

	if (motor == MOTORA) {
		pin1 = PIN_A1;
		pin2 = PIN_A2;
		pinE = PIN_ENA;
	}
	else if (motor == MOTORB) {
		pin1 = PIN_B1;
		pin2 = PIN_B2;
		pinE = PIN_ENB;
	}

	digitalWrite(pin1, LOW);
	digitalWrite(pin2, LOW);
	digitalWrite(pinE, LOW);
}

void stop() {
	Serial.println("Stop Car");
	stopMotor(MOTORA);
	stopMotor(MOTORB);
}

void go(uint8_t direction, uint8_t input) {
	Serial.print("Go: ");
	
	if (direction == FORWARD || direction == BACKWARD) {
		moveMotor(MOTORA, direction, input);
		moveMotor(MOTORB, direction, input);
	}
	else if (direction = LEFT) {
		stopMotor(MOTORA);
		moveMotor(MOTORB, FORWARD, input);
	}
	else if (direction == RIGHT) {
		stopMotor(MOTORB);
		moveMotor(MOTORA, FORWARD, input);
	}
}


