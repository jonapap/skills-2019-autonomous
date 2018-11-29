#include "Color_corect.h"

PRIZM prizm;
EXPANSION exc;
OmniRobot robot(47.5, 7.5, 90);
//OmniRobot robot(0,0,0);

Square redSquare( { 28, 82.5 }, 90);
Square blueSquare( { 68, 82.5 }, 90);
Square yellowSquare( { 49, 82.5 }, 90);

Block blocks[] = { Block( { 66.75, 5.5 }, 270, RIGHTAPPROACH, yellowSquare), //block 1
Block( { 80, 33 }, 0, LEFTAPPROACH, redSquare), //block 2
Block( { 4.25, 63.5 }, 180, LEFTAPPROACH, redSquare),  //block 6
Block( { 15.5, 33 }, 180, RIGHTAPPROACH, yellowSquare), //block 5
Block( { 29.5, 5.5 }, 270, LEFTAPPROACH, blueSquare), //block 4
Block( { 90.75, 63.5 }, 0, RIGHTAPPROACH, blueSquare) //block 3
		};

void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(1, 1);
	robot.invertMotor(2, 1);

	robot.advanceUntilColor(100, 0, robot.blue);

	robot.advanceUntilColor(100, 270, robot.blue, true);

	robot.advanceRelative(6, 100, 90);

	//cycleBlocks();
	//prizm.PrizmEnd();
}

void loop() {
}

void cycleBlocks() {
	for (Block &b : blocks) {
		robot.goToPosition(b.getLastPoint(), 100, 100);
		robot.goToHeading(b.heading, 100);

		robot.advanceUntilLine(100, b.approachSide == RIGHTAPPROACH ? 90 : 270);
		robot.advanceRelative(1.5, 100, 90);
		robot.goToPingDistance(100, 2);

		grabBlock();

		robot.setPosition(b.getRobotLinePosition());

		/*Square &s = b.getSquare();
		 robot.goToPosition(s.getApproachPosition(), 100, 100);
		 robot.goToHeading(mod(s.getApproachHeading() + 180, 360), 100);*/
		//depositBlock();
	}
}

void grabBlock() {

}

void depositBlock() {
	robot.advanceRelative(10, 100, 180);
	robot.gripperHor(OPEN);
	delay(1000);
	robot.advanceRelative(10, 100, 180);
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
	robot.goToPosition(x, y, 100);
	//robot.goToHeading(90, 100);

}

void printXY() {
	Serial.println(robot.getPosition().x);
	Serial.println(robot.getPosition().y);
	Serial.println(robot.getHeading());
	while (prizm.readStartButton() == 0)
		;
	delay(500);
}

void serialBlocks() {
	while (Serial.available() == 0)
		;
	int n = Serial.parseInt();

	robot.goToPosition(blocks[n].getLastPoint(), 100, 100);
	/*robot.goToHeading(blocks[n].getApproachHeading(), 100);
	 grabBlock();

	 robot.setPosition(blocks[n].getRobotLinePosition());
	 robot.setHeading(blocks[n].heading);

	 Square &s = blocks[n].getSquare();
	 robot.goToPosition(s.getApproachPosition(), 100, 100);
	 robot.goToHeading(s.getApproachHeading(), 100);
	 depositBlock();*/
}
