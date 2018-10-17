#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "TETRIX_PRIZM/PRIZM.h"
#include "Color_sensor/GroveColorSensor.h"

struct RGB {
	int red, green, blue;

	boolean isColor(const RGB &color, int error);

};

struct Position {
	double x, y;
};

class Robot {
public:

	void advanceIN(double distance, double motorSpeed);
	void turn(double degrees, int speed);
	void advanceUntilLine(int speed, boolean stop = true);
	void alignWithLine(int speed, int direction);
	void invertMotor(int motor, int invert);
	void gripperOpen(int direction);
	void gripperUp(int direction);
	void advanceUntilPing(int speed, int distance);
	void setPosition(double x, double y);
	void setPosition(Position p);
	void goToPosition(double x, double y, int speed);
	void goToPosition(double x, double y, int speed, int turningSpeed);
	void goToPosition(Position p, int speed);
	void goToPosition(Position p, int speed, int turningSpeed);
	Position getPosition();
	double getHeading();
	void goToHeading(double heading, int speed);
	void setHeading(double heading);
	RGB readColor();
	void rampSpeed(unsigned int speed, unsigned int time);

	static const RGB black;
	static const RGB red;
	static const RGB blue;
	static const RGB yellow;

private:
	PRIZM prizm;

	double wheelcirIN = 6.3;
	double turnvalue = 12.3;

	const unsigned int pingSensor = 4;
	const unsigned int lineSensor = 3;
	const unsigned int gripperHorizontal = 1;
	const unsigned int gripperVertical = 2;

	int motor1invert = 1;
	int motor2invert = 1;

	double heading = 90;
	double x = 0;
	double y = 0;

	const int colorError = 10;

	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
	long getEncoderCount(int motor);
	void updatePosition(long encoder1, long encoder2);
	void updateAngle(long encoder1, long encoder2);
};

#endif
