#include "Color_corect.h"

PRIZM prizm;
Robot robot;

Square redSquare( { 28, 88 }, 90);
Square blueSquare( { 68, 88 }, 90);
Square yellowSquare( { 49, 88 }, 90);

Block blocks[] = { Block( { 66.75, 5.5 }, 270, RIGHTAPPROACH, yellowSquare), //block 1
Block( { 80, 33 }, 0, LEFTAPPROACH, redSquare), //block 2
Block( { 90.75, 63.5 }, 0, RIGHTAPPROACH, blueSquare), //block 3
Block( { 29.5, 5.5 }, 270, LEFTAPPROACH, blueSquare), //block 4
Block( { 15.5, 33 }, 180, RIGHTAPPROACH, yellowSquare), //block 5
Block( { 4.25, 63.5 }, 180, LEFTAPPROACH, redSquare)  //block 6
		};

void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(2, 1);
	robot.setPosition(47.5, 8);

	//grabBlock(blocks[2]);
	cycleBlocks();
	//robot.alignWithLine(25);
}

void loop() {
	/*
	 while (Serial.available() == 0)
	 ;
	 double x = Serial.parseFloat();
	 Serial.println(x);
	 while (Serial.available() == 0)
	 ;
	 double y = Serial.parseFloat();
	 Serial.println(y);
	 robot.goToPosition(x, y, 100);*/
}

void serialBlocks() {
	while (Serial.available() == 0)
		;
	int n = Serial.parseInt();

	robot.goToPosition(blocks[n].getLastPoint(), 100, 100);
	robot.goToHeading(blocks[n].getApproachHeading(), 100);
	grabBlock(blocks[n]);

	robot.setPosition(blocks[n].getRobotLinePosition());
	robot.setHeading(blocks[n].heading);

	Square &s = blocks[n].getSquare();
	robot.goToPosition(s.getApproachPosition(), 100, 100);
	robot.goToHeading(s.getApproachHeading(), 100);
	depositBlock();
}

void cycleBlocks() {
	for (Block &b : blocks) {
		robot.goToPosition(b.getLastPoint(), 100, 100);
		robot.goToHeading(b.getApproachHeading(), 100);

		grabBlock(b);

		robot.advanceIN(5, 100);

		robot.gripperVert(DOWN);

		robot.setPosition(b.getRobotLinePosition());
		robot.setHeading(b.heading + 180);

		Square &s = b.getSquare();
		robot.goToPosition(s.getApproachPosition(), 100, 100);
		robot.goToHeading(s.getApproachHeading(), 100);
		depositBlock();
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
	robot.goToPosition(x, y, 100);
	robot.goToHeading(90, 100);

}

void printXY() {
	Serial.println(robot.getPosition().x);
	Serial.println(robot.getPosition().y);
	Serial.println(robot.getHeading());
	while (prizm.readStartButton() == 0)
		;
	delay(500);
}

void grabBlock(Block &b) {
	robot.gripperVert(UP);
	robot.gripperHor(OPEN);

	robot.alignWithLine(25);

	robot.advanceIN(5, 50);
	robot.turn(-90 * b.approachSide, 50);

	robot.advanceUntilPing(50, 2);
	//robot.setPosition(b.getRobotLinePosition());

	robot.advanceIN(-10, 50);

	robot.turn(180, 50);
	//robot.turn(90, 50);
	//robot.advanceIN(-2, 50);
	//robot.turn(90, 50);

	robot.advanceIN(-9, 50);

	robot.gripperHor(CLOSE);
	delay(500);
	//robot.advanceIN(-10, 100);

}

void depositBlock(){
	robot.turn(180, 50);
	robot.gripperHor(OPEN);
	robot.advanceIN(5, 100);
}
