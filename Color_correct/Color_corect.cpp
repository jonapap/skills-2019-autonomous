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
Block( { 4.25, 63.5 }, 0, LEFTAPPROACH, redSquare)  //block 6
		};

void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(2, 1);
	robot.setPosition(47.5, 8);
}

void loop() {
	cycleBlocks();
}

void serialBlocks() {
	while (Serial.available() == 0)
		;
	int n = Serial.parseInt();

	Position p = blocks[n].getLastPoint();
	robot.goToPosition(p.x, p.y, 300);
	robot.goToHeading(blocks[n].getApproachHeading(), 100);
	grabBlock(blocks[n].approachSide);
	Position p2 = blocks[n].getRobotLinePosition();

	robot.setPosition(p2.x, p2.y);
	robot.setHeading(blocks[n].heading);

	Square &s = blocks[n].getSquare();
	robot.goToPosition(s.getApproachPosition(), 300, 100);
	robot.goToHeading(s.getApproachHeading(), 100);
	depositBlock();
}

void cycleBlocks() {
	for (Block b : blocks) {
		robot.goToPosition(b.getLastPoint(), 300);
		robot.goToHeading(b.getApproachHeading(), 100);

		grabBlock(b.approachSide);

		robot.setPosition(b.getRobotLinePosition());
		robot.setHeading(b.heading);

		Square &s = b.getSquare();
		robot.goToPosition(s.getApproachPosition(), 300, 100);
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
	robot.goToPosition(x, y, 300);
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

void grabBlock(int side) {
	robot.advanceUntilLine(100, true);
	robot.advanceIN(6, 100);
	delay(200);
	robot.alignWithLine(100, -side);
	robot.advanceUntilPing(100, 2);
	//robot.advanceIN(-10, 100);

}
