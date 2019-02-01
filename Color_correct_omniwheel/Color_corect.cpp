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
		robot.gripperVert(UP);
		robot.gripperHor(OPEN);
		goToBlock(b);
		robot.goToHeading(b.heading, 100);

		alignWithLine(b.approachSide);

		grabBlock();

		robot.goToPingDistance(100, 4, true);

		robot.setPosition(b.getRobotAlignedPosition());
		robot.setHeading(b.heading);

		robot.advanceRelative(4, 100, 180);

		Square &s = b.getSquare();
		goToSquare(b);
		robot.goToHeading(s.getApproachHeading(), 100);

		s.align(robot);
		robot.setPosition(s.getRobotAlignedPosition());

		depositBlock(s);
	}
}

void goToBlock(Block &b){
	for(int i = 0; i<b.numBeforePos; i++){
			robot.goToPosition(b.beforePosition[i], 100);
	}

	robot.goToPosition(b.getLastPoint(), 100);
}

void goToSquare(Block &b){
	for(int i = 0; i<b.numAfterPos; i++){
		robot.goToPosition(b.afterPosition[i], 100);
	}

	robot.goToPosition(b.getSquare().getApproachPosition(), 100);
}

void depositBlock(Square &s){
	robot.advanceRelative(s.numOfBlocks == 0 ? 5 : 0, 100, s.numOfBlocks == 0 ? 0 : 180);
	s.numOfBlocks++;

	robot.gripperVert(DOWN);
	delay(3000);
	robot.gripperHor(OPEN);
	delay(GRIPPER_TIME);

	robot.advanceRelative(5, 100, 180);
}

void grabBlock() {
	robot.advanceRelative(4, 100, 0);
	robot.gripperHor(CLOSE);
	delay(GRIPPER_TIME);
	robot.advanceRelative(6, 100, 180);
}

void alignWithLine(int side) {
	robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 90 : 270);

	while (robot.readLineSensor(robot.lineSensorFront) == 0)
		;

	robot.goToPingDistance(50, 5);

	robot.advanceRelative(3, 50, 270);

	robot.goInRelativeDirection(50, 90);

	int lineFront = 0;
	while (robot.readLineSensor(robot.lineSensorMiddle) == 0) {
		lineFront =
				lineFront == 1 ?
						1 : robot.readLineSensor(robot.lineSensorFront);
	}

	if (lineFront == 0) {
		robot.turnInDirection(-50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		robot.stopAllMotors();
	} else {
		robot.turnInDirection(50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		while (robot.readLineSensor(robot.lineSensorFront) == 1)
			;
		robot.turnInDirection(-50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		robot.stopAllMotors();
	}

	robot.goToPingDistance(50, 4, true);

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
