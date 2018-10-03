// Do not remove the include below
#include "Color_corect.h"

PRIZM prizm;
Robot robot;

//The setup function is called once at startup of the sketch
void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(2, 1);
	robot.setPosition(47, 8);

	robot.goToLocation(58, 14, 300);
	grabBlock();
	goToYellow();

	/*
	 robot.goToLocation(66.5, 17, 300);
	 goToYellow();
	 robot.goToLocation(28, 33, 300);
	 goToYellow();

	 robot.goToLocation(16, 64, 300);
	 goToRed();
	 robot.goToLocation(68, 33, 300);
	 goToRed();

	 robot.goToLocation(78, 63, 300);
	 goToBlue();
	 robot.goToLocation(29, 19, 300);
	 goToBlue();*/

	//robot.advanceUntilLine(100, Robot::yellow, 5);
	//prizm.setMotorSpeeds(300, 300);
	//robot.rampSpeed(720, 1000);
	//robot.advanceCM(30, 300);
	//prizm.setMotorPowers(100,100);
}

void loop() {
}

void goToRed() {
	robot.goToLocation(28, 88, 300);
}

void goToYellow() {
	robot.goToLocation(49, 88, 300);
}

void goToBlue() {
	robot.goToLocation(68, 88, 300);
}

void grabBlock() {
	robot.setHeading(0, 100);
	Serial.println("");
	Serial.println("");
	Serial.println("");
	robot.advanceUntilLine(100, true);
	Serial.println(robot.x);
	Serial.println(robot.y);
	delay(5000);
	robot.advanceIN(6, 100);
	robot.alignWithLine(100, -1);
	robot.advanceUntilPing(100, 2);
	delay(1000);
	robot.advanceIN(-10, 100);

	robot.heading = 270;
}
