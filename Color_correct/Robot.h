#ifndef ROBOT_H
#define ROBOT_H

#define IN_RANGE(x, y, r) abs(x-y) <= r

#include <Arduino.h>
#include "TETRIX_PRIZM/PRIZM.h"
#include "Color_sensor/GroveColorSensor.h"
#include "DEBUG.h"

struct RGB {
	int red,green,blue;

	int getColor(const RGB colors[], unsigned int size);

};
/*
int RGB::getColor(const RGB colors[], unsigned int size){
	int error = 10;

	for(int i = 0; i < size; i++){
		RGB color = colors[i];

		if(IN_RANGE(color.red, red, error) && IN_RANGE(color.green, green, error) && IN_RANGE(color.blue, blue, error)){
			return i;
		}
	}

	throw "Color not in list!";
}*/


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
	void setPosition (double x, double y);
	void goToLocation(double x, double y, int speed);
	void setHeading(double heading, int speed);
	RGB readColor();
	void rampSpeed(unsigned int speed, unsigned int time);


private:
	PRIZM prizm;

	const double wheelcirIN = 20;
	const double turnvalue = 5.82;

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

	static constexpr RGB colors[] = {
			{0,0,0},
			{255,0,0},
			{255,255,0},
			{0,0,255}
	};

	void waitForMotors();
	void waitForEncoder(long target1, long target2);
	boolean checkForEncoder(long target1, long target2, long diff);
	long getEncoderCount(int motor);
	void updatePosition(long encoder1, long encoder2);
	void updateAngle(long encoder1, long encoder2);
};

#endif
