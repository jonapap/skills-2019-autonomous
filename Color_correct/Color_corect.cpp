#include "Color_corect.h"

PRIZM prizm;
Robot robot;

Block blocks[] = { Block( { 66.75, 5.5 }, 270, RIGHTAPPROACH, YELLOWSQUARE), //block 1
Block( { 80, 33 }, 0, LEFTAPPROACH, REDSQUARE), //block 2
Block( { 90.75, 63.5 }, 0, RIGHTAPPROACH, BLUESQUARE), //block 3
Block( { 29.5, 5.5 }, 270, LEFTAPPROACH, BLUESQUARE), //block 4
Block( { 15.5, 33 }, 180, RIGHTAPPROACH, YELLOWSQUARE), //block 5
Block( { 4.25, 63.5 }, 0, LEFTAPPROACH, REDSQUARE)  //block 6
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
	robot.goToLocation(p.x, p.y, 300);
	robot.goToHeading(blocks[n].getApproachHeading(), 100);
	grabBlock(blocks[n].approachSide);
	Position p2 = blocks[n].getRobotLinePosition();

	robot.setPosition(p2.x, p2.y);
	robot.setHeading(blocks[n].heading);
	goToSquare(blocks[n].color);
}

void cycleBlocks() {
	for (Block b : blocks) {
		Position p = b.getLastPoint();
		robot.goToLocation(p.x, p.y, 300);
		robot.goToHeading(b.getApproachHeading(), 100);
		grabBlock(b.approachSide);

		Position p2 = b.getRobotLinePosition();

		DEBUG_PRINTLN("Position :");
		DEBUG_PRINTLN(p2.x);
		DEBUG_PRINTLN(p2.y);

		robot.setPosition(p2.x, p2.y);
		robot.setHeading(b.heading);
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

void grabBlock(int side) {
	robot.advanceUntilLine(100, true);
	robot.advanceIN(6, 100);
	delay(200);
	robot.alignWithLine(100, -side);
	robot.advanceUntilPing(100, 2);
	//robot.advanceIN(-10, 100);

}
