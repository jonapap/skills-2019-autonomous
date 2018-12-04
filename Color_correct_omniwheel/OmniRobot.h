#ifndef ROBOT_H
#define ROBOT_H

#define OPEN 180
#define CLOSE 0
#define DOWN 180
#define UP 0

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

class OmniRobot {
public:

	OmniRobot(double x, double y, double heading) :
			heading(heading), x(x), y(y) {
	}

	void advanceRelative(double distance, double motorSpeed, double angle = 0);
	void advanceAbsolute(double distance, double motorSpeed, double angle = 0);
	void goInRelativeDirection(double motorSpeed, double angle);
	void goInAbsoluteDirection(double motorSpeed, double angle);
	void turn(double degrees, int speed);
	void turnInDirection(double motorSpeed);
	void advanceUntilLine(int speed, double direction, boolean stop = true);
	void advanceUntilColor(int speed, double direction, RGB color, int colorError = 10, boolean invert = false, boolean stop = true);
	void invertMotor(int motor, int invert);
	void gripperHor(int direction);
	void gripperVert(int direction);
	void goToPingDistance(int speed, int distance);
	void setPosition(double x, double y);
	void setPosition(Position p);
	void goToPosition(double x, double y, int speed);
	void goToPosition(Position p, int speed);
	Position getPosition();
	double getHeading();
	void goToHeading(double heading, int speed);
	void setHeading(double heading);
	RGB readColor();
	void rampSpeed(unsigned int speed, unsigned int time);
	void stopAllMotors();
	void alignWithPing();

	friend void setup();
	friend void loop();

private:
	PRIZM prizm;
	EXPANSION exc;

	int dcControllerAddr = 1;

	const double wheelcirIN = 4 * PI;
	double turnvalue = 15.0;

	const unsigned int pingSensorRight = 3;
	const unsigned int pingSensorLeft = 4;
	const unsigned int lineSensor = 5;

	const unsigned int gripperHorizontal = 2;
	const unsigned int gripperVertical = 6;

	int motorinvert[4] = { 1, 1, 1, 1 };

	double heading;
	double x;
	double y;

	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
	long getEncoderCount(int motor);
	void updatePosition(long encoder1, long encoder2);
	void updateAngle(long encoder1, long encoder2);
	void holdMotor(int motor);
	void holdAllMotors();
	boolean readLineSensor(int sensor);
	int getMotorInvert(int motor);
	double getRelativeAngle(double angle);
	double getAbsoluteAngle(double angle);
};

#endif
