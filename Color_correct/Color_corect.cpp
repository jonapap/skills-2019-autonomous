// Do not remove the include below
#include "Color_corect.h"

PRIZM prizm;
Robot robot;

Block blocks[] = {
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE), //block 1
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE), //block 2
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE), //block 3
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE), //block 4
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE), //block 5
		Block({ 80, 33 }, 0, LEFTAPPROACH, YELLOWSQUARE)  //block 6
};

//The setup function is called once at startup of the sketch
void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(2, 1);
	robot.setPosition(47.5, 8);


	/*
	 robot.goToLocation(58, 14, 300);
	 //grabBlock();
	 goToYellow();
	 printXY();

	 robot.advanceIN(30, 100);
	 */
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
	testLocation();
}

void cycleBlocks() {
	for (Block b : blocks) {
		Position p = b.getLastPoint();
		robot.goToLocation(p.x, p.y, 300);
		robot.setHeading(b.getApproachHeading());
		grabBlock();
		goToSquare(b.color);
	}
}

void testLocation() {
	Serial.println("X :");
	while (Serial.available() == 0)
		;
	double x = Serial.parseFloat();
	Serial.println("Y :");
	while (Serial.available() == 0)
		;
	double y = Serial.parseFloat();
	robot.goToLocation(x, y, 300);
	robot.goToHeading(90, 100);

}

void goToSquare(int color) {
	switch (color) {
	case REDSQUARE:
		robot.goToLocation(28, 88, 300);
		break;
	case YELLOWSQUARE:
		robot.goToLocation(49, 88, 300);
		break;
	case BLUESQUARE:
		robot.goToLocation(68, 88, 300);
		break;
	}
}

void printXY() {
	Serial.println(robot.getPosition().x);
	Serial.println(robot.getPosition().y);
	Serial.println(robot.getHeading());
	while (prizm.readStartButton() == 0)
		;
	delay(500);
}

void grabBlock() {
	robot.goToHeading(0, 100);
	robot.advanceUntilLine(100, true);
	printXY();
	robot.advanceIN(6, 100);
	printXY();
	robot.alignWithLine(100, -1);
	printXY();

	robot.advanceUntilPing(100, 2);
	printXY();
	robot.advanceIN(-10, 100);
	printXY();

}
