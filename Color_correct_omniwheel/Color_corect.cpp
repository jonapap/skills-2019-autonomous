#include "Color_corect.h"

#include "Layouts/Layout1.h"

PRIZM prizm;
EXPANSION exc;

const unsigned int startBlock = 4;

void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	setupCourt();

	prizm.PrizmBegin();
	robot.invertMotor(1, 1);
	robot.invertMotor(2, 1);

	long timeStart = millis();
	cycleBlocks();

	if(millis()-timeStart > 600000){
		while(true){
			prizm.setRedLED(HIGH);
			delay(500);
			prizm.setRedLED(LOW);
			delay(500);
		}
	} else {
		prizm.PrizmEnd();
	}
}

void loop() {
	/*RGB color = robot.readColor();
	 Serial.println(color.red);
	 Serial.println(color.green);
	 Serial.println(color.blue);
	 Serial.println("");*/
}

void cycleBlocks() {
	int firstTime = true;
	for (unsigned int i = startBlock; i<(sizeof(blocks)/sizeof(blocks[0])); i++) {
		Block &b = blocks[i];

		robot.gripperVert(UP);
		robot.gripperHor(OPEN);
		if (!firstTime || i == 0) {
			goToBlock(b);
			robot.goToHeading(b.heading, 100);
		}

		alignWithLine(b.approachSide);

		grabBlock();

		robot.goToPingDistance(100, 4, true);

		robot.setPosition(b.getRobotAlignedPosition());
		robot.setHeading(b.heading);

		robot.advanceRelative(4, 100, 180);

		Square &s = b.getSquare();
		goToSquare(b);
		robot.goToHeading(s.getApproachHeading(), 100);

		robot.alignWithSquare(s);
		robot.setPosition(s.getRobotAlignedPosition());

		depositBlock(s);

		firstTime = false;
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

	robot.gripperVert(DOWN);
	delay(500);
	robot.gripperVert(NEUTRAL);

	robot.gripperHor(CLOSE);
	delay(GRIPPER_TIME);

	robot.gripperVert(UP);

	robot.advanceRelative(6, 100, 180);
}

void alignWithLine(int side) {
	EncoderValues encvals = robot.getEncoderValues();

	robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 90 : 270); //go in direction of line

	unsigned long timeStart = millis();
	while (robot.readLineSensor(robot.lineSensorFront) == 0) {
		if (millis() - timeStart > 10000) {
			robot.goToPosition(encvals, 100); //go to original position

			robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 270 : 90); //check if line is in the other direction
			unsigned long timeStart2 = millis();
			while (robot.readLineSensor(robot.lineSensorFront) == 0
					&& millis() - timeStart2 < 5000)
				;

			if (robot.readLineSensor(robot.lineSensorFront) == 1) //if the line is indeed there, get out of the loop
				break;

			robot.goToPosition(encvals, 100); //go to original position

			robot.advanceRelative(4, 100, 0); //advance a bit forward

			encvals = robot.getEncoderValues(); //save position

			robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 90 : 270); //search again for the line
			timeStart = millis(); //reset time
		}
	}

	robot.goToPingDistance(50, 5); //advance a bit

	robot.advanceRelative(3, 50, 270); //go right

	robot.goInRelativeDirection(50, 90);

	int lineFront = 0;
	while (robot.readLineSensor(robot.lineSensorMiddle) == 0) { //looping until middle sensor hits. Also saves if the front sensor hit the line before the middle
		lineFront =
				lineFront == 1 ?
						1 : robot.readLineSensor(robot.lineSensorFront);
	}

	if (lineFront == 0) { //if front sensor is at the right of the line
		robot.turnInDirection(50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		robot.stopAllMotors();
	} else { //if the front sensor is at the left of the line
		robot.turnInDirection(-50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		while (robot.readLineSensor(robot.lineSensorFront) == 1)
			;
		robot.turnInDirection(50);
		while (robot.readLineSensor(robot.lineSensorFront) == 0)
			;
		robot.stopAllMotors();
	}

	robot.goToPingDistance(50, 4, true);

}

void alignWithSquare(Square &s){

	robot.advanceUntilColor(50, 0, s.getColor(), s.getColorError());
	EncoderValues val = robot.getEncoderValues();

	robot.advanceUntilColor(50, 270, s.white, 15);
	robot.advanceRelative(2, 100, 90);
	double distanceX = robot.getDistance(val).distance;

	val = robot.getEncoderValues();
	robot.advanceUntilColor(50, 180, s.white, 15);
	robot.advanceUntilColor(50, 0, s.getColor(), 15);
	double distanceY = robot.getDistance(val).distance;

	double angle = toDegrees(atan2(distanceY, distanceX));

	Serial.println("Angle:");
	Serial.println(angle);

	robot.turn(-angle, 100);

	robot.advanceUntilColor(50, 90, s.getColor(), s.getColorError());
	robot.advanceUntilColor(50, 270, s.white, 15);
	robot.advanceRelative(5, 100, 90);

	robot.setHeading(s.getApproachHeading());
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
