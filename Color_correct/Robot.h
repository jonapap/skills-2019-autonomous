#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "TETRIX_PRIZM/PRIZM.h"
#include "Color_sensor/GroveColorSensor.h"
#include "DEBUG.h"

struct RGB {
	int red,green,blue;
};

class Robot {
public:

	void advanceIN(double distance, double motorSpeed, boolean stop = true);
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
	void setPosition (int x, int y);
	void goToLocation(int x, int y, int speed);
	void setHeading(double heading, int speed);
	RGB readColor();
	void rampSpeed(unsigned int speed, unsigned int time);


private:
	PRIZM prizm;

	const double wheelcirIN = 20;
	const double turnvalue = 6.625;

	const unsigned int lineSensorFront = 3;
	const unsigned int lineSensorBack = 4;
	const unsigned int pingSensor = 5;
	const unsigned int gripperHorizontal = 1;
	const unsigned int gripperVertical = 2;

	int motor1invert = 1;
	int motor2invert = 1;

	double heading = 90;
	double x = 0;
	double y = 0;


	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
	long getEncoderCount(int motor);
	void updatePosition(long encoder1, long encoder2);
	void updateAngle(long encoder1, long encoder2);
};

#endif
