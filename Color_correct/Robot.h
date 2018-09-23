#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "library/TETRIX_PRIZM/PRIZM.h"

class Robot {
public:
	Robot(PRIZM p, double w, double t) :
			prizm(p), wheelcirCM(w), turnvalue(t) {
	}
	;

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
	void setPosition (int x, int y);
	void goToLocation(int x, int y, int speed);
	void setHeading(int heading, int speed);

private:
	PRIZM prizm;
	const double wheelcirCM;
	const double turnvalue;

	const unsigned int lineSensorFront = 3;
	const unsigned int lineSensorBack = 4;
	const unsigned int pingSensor = 5;
	const unsigned int gripperHorizontal = 6;
	const unsigned int gripperVertical = 6;

	uint8_t motor1invert = 1;
	uint8_t motor2invert = 1;

	unsigned int heading = 90;
	int x = 0;
	int y = 0;


	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
};

#endif
