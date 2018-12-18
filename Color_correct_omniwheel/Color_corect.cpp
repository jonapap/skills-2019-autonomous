#include "Color_corect.h"

#include "Layouts/Layout1.h"

PRIZM prizm;
EXPANSION exc;


void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	setupCourt();

	prizm.PrizmBegin();
	robot.invertMotor(1, 1);
	robot.invertMotor(2, 1);


	cycleBlocks();
	prizm.PrizmEnd();
}

void loop() {
	/*RGB color = robot.readColor();
	 Serial.println(color.red);
	 Serial.println(color.green);
	 Serial.println(color.blue);
	 Serial.println("");*/
}

void cycleBlocks() {
	for (Block &b : blocks) {
		goToBlock(b);
		robot.goToHeading(b.heading, 100);

		b.align(robot);

		robot.alignWithPing();

		grabBlock();

		robot.setPosition(b.getRobotAlignedPosition());

		Square &s = b.getSquare();
		goToSquare(b);
		robot.goToHeading(s.getApproachHeading(), 100);

		s.align(robot);
		robot.setPosition(s.getRobotAlignedPosition());

		depositBlock(s);
	}
}

void goToBlock(Block &b){
	if(b.hasBeforePos){
		robot.goToPosition(b.beforePosition, 100);
	}

	robot.goToPosition(b.getLastPoint(), 100);
}

void goToSquare(Block &b){
	if(b.hasAfterPos){
			robot.goToPosition(b.afterPosition, 100);
		}
	robot.goToPosition(b.getSquare().getApproachPosition(), 100);
}

void depositBlock(Square &s){
	robot.advanceRelative(2, 100, s.numOfBlocks == 0 ? 0 : 180);

	s.numOfBlocks++;
}

void grabBlock() {

}
/*
 void depositBlock() {
 robot.advanceRelative(10, 100, 180);
 robot.gripperHor(OPEN);
 delay(1000);
 robot.advanceRelative(10, 100, 180);
 }*/

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

	robot.goToPosition(blocks[n].getLastPoint(), 100);
	/*robot.goToHeading(blocks[n].getApproachHeading(), 100);
	 grabBlock();

	 robot.setPosition(blocks[n].getRobotLinePosition());
	 robot.setHeading(blocks[n].heading);

	 Square &s = blocks[n].getSquare();
	 robot.goToPosition(s.getApproachPosition(), 100, 100);
	 robot.goToHeading(s.getApproachHeading(), 100);
	 depositBlock();*/
}
