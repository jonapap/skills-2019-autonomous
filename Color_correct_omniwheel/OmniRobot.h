#ifndef ROBOT_H
#define ROBOT_H

#define OPEN 0 //120
#define CLOSE 180 //70
#define GRIPPER_TIME 8000

#define DOWN 180 //100
#define UP 0 //125
#define NEUTRAL 90

#include <Arduino.h>
#include "TETRIX_PRIZM/PRIZM.h"
#include "Color_sensor/GroveColorSensor.h"
#include "Wire.h"
#include "Types.h"
#include "Square.h"

class Square;

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
	boolean advanceUntilColor(int speed, double direction, RGB color, int colorError = 20, boolean invert = false, boolean stop = true, unsigned long timeout = 0);
	void turnUntilColor(int speed, RGB color, int colorError = 20, boolean invert = false, boolean stop = true);
	void invertMotor(int motor, int invert);
	void gripperHor(int direction);
	void gripperVert(int direction);
	void goToPingDistance(int speed, int distance, int back = false);
	void setPosition(double x, double y);
	void setPosition(Position p);
	void goToPosition(double x, double y, int speed);
	void goToPosition(Position p, int speed);
	void goToPosition(EncoderValues values, int speed);
	Position getPosition();
	double getHeading();
	void goToHeading(double heading, int speed);
	void setHeading(double heading);
	RGB readColor();
	void rampSpeed(unsigned int speed, unsigned int time);
	void stopAllMotors();
	void alignWithPing();
	boolean readLineSensor(int sensor);
	EncoderValues getEncoderValues();
	Vector getDistance(EncoderValues values);
	double readPing(int pin);


	friend void setup();
	friend void loop();

	const unsigned int pingSensorRight = 3;
	const unsigned int pingSensorLeft = A1;
	const unsigned int lineSensorFront = A2;
	const unsigned int lineSensorMiddle = 4;

	const unsigned int gripperHorizontal = 2;
	const unsigned int gripperVertical = 1;

	friend void alignWithLine(int side);

	const double wheelcirIN = (4 * PI)-0.2;
	const double turnvalue = 11.65; //15.0 for bigger robot
	const double robotradiusIN = (turnvalue*wheelcirIN)/(8*PI);

private:
	PRIZM prizm;
	EXPANSION exc;

	int dcControllerAddr = 1;

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
	int getMotorInvert(int motor);
	double getRelativeAngle(double angle);
	double getAbsoluteAngle(double angle);
};

#endif
