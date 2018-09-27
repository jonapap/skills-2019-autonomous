#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "TETRIX_PRIZM/PRIZM.h"
#include "Color_sensor/GroveColorSensor.h"

class Robot {
public:

	void advanceCM(double distance, double motorSpeed, boolean stop = true);
	void turn(double degrees, int speed);
	void advanceUntilLine(int speed, boolean stop = true);
	void alignWithLine(int speed, int direction);
	void forwardAndTurn(double turnLenght, double turnDistance, int speed,
			boolean direction, boolean back);
	void advanceToWall(int speed);
	void invertMotor(int motor, int invert);
	void gripperOpen(int direction);
	void gripperUp(int direction);
	void advanceUntilPing(int speed, int distance);
	void readColor(int &red, int &green, int &blue);
	void rampSpeed(unsigned int speed, unsigned int time);

private:
	PRIZM prizm;
	const double wheelcirCM = 40;
	const double turnvalue = 40;

	const unsigned int lineSensorFront = 3;
	const unsigned int lineSensorBack = 4;
	const unsigned int pingSensor = 5;
	const unsigned int gripperHorizontal = 1;
	const unsigned int gripperVertical = 2;

	uint8_t motor1invert = 1;
	uint8_t motor2invert = 1;

	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
};

#endif
