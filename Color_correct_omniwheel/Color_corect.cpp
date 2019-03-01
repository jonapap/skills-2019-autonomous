#include "Color_corect.h"

#include "Layouts/Layout1.h"

PRIZM prizm;
EXPANSION exc;

const unsigned int startBlock = 0;

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
	/*while(Serial.available() == 0);
	int data = Serial.parseInt();
	Serial.println(data);
	prizm.setServoPosition(robot.gripperHorizontal, data);*/
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

		alignWithSquare(s);
		robot.setPosition(s.getRobotAlignedPosition());

		depositBlock(s);

		firstTime = false;
	}
}

void goToBlock(Block &b){
	for(int i = 0; i<b.numBeforePos; i++){
			robot.goToPosition(b.beforePosition[i], 200);
	}

	robot.goToPosition(b.getLastPoint(), 200);
}

void goToSquare(Block &b){
	for(int i = 0; i<b.numAfterPos; i++){
		robot.goToPosition(b.afterPosition[i], 200);
	}

	robot.goToPosition(b.getSquare().getApproachPosition(), 200);
}

void depositBlock(Square &s){
	if(s.numOfBlocks == 0){
		robot.advanceRelative(3, 100, 0);
		robot.gripperVert(DOWN);
		delay(4000);
		robot.gripperHor(OPEN);
		delay(GRIPPER_TIME);
		robot.advanceRelative(5, 100, 180);
	} else {
		robot.advanceRelative(3, 100, 180);
		robot.gripperVert(DOWN);
		delay(4000);
		robot.advanceRelative(3.75, 100, 0);
		robot.gripperHor(OPEN);
		delay(GRIPPER_TIME);
		robot.advanceRelative(5, 100, 180);
	}
	s.numOfBlocks++;
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

void alignWithSquare(Square &s) {
	boolean reached = false;
	int count = 0;
	EncoderValues val = robot.getEncoderValues(); //save robot's position

	do {
		reached = robot.advanceUntilColor(50, 0, s.getColor(), s.getColorError(), 0,
				0, 5000); //try to reach square color with a 5000 ms timeout

		if (reached == false && (count == 0 || count == 1)) { //if we didn't hit the block and we are at our first or second try
			robot.goToPosition(val, 100); //return to orignial position
			robot.advanceRelative(8, 100, count == 0 ? 90 : 270); //try to move a bit to left(at first try) or right(at second try)
		} else if (reached == false && count == 2) { //if we still didn't reach the square at the third try
			robot.goToPosition(val, 100); //return to original position
			robot.setPosition(s.getApproachPosition()); //make sure the robot's position is correct
			robot.goToPosition(s.getRobotAlignedPosition(), 100); //go to where we think the square is
			break; //get out of the loop
		}

		count++; //increase the amount of times the robot tried to rach the square
	} while (reached == false); //stay in the loop as long as the robot didn't reach the square (or we didn't break out of the loop)

	if (reached == true) { //continue aligning with square only if we have reached the square
		robot.advanceRelative(2, 50, 0); //the following four lines make sure the robot is at a certain distance from the bottom and right side of the square
		robot.advanceUntilColor(50, 270, Square::white, 15);
		robot.advanceRelative(4, 50, 90);
		robot.advanceUntilColor(50, 180, Square::white, 15);

		int offset = 2; //how much the robot will advance from bottom of square

		robot.advanceRelative(offset, 50, 0); //advance the specified offset
		robot.turnUntilColor(50, Square::white, 15); //turn until the color sees white

		int angle = mod(toDegrees(atan2(robot.robotradiusIN, robot.robotradiusIN - offset)),
				360); //calculate angle required base on offset and robot radius

		robot.turn(-(angle-3), 50); //turn the calculated amount (with a small offset)

		robot.advanceUntilColor(50, 180, Square::white, 15); //following four lines will place robot at the bottom center of the robot
		robot.advanceUntilColor(50, 0, s.getColor(), s.getColorError());
		robot.advanceUntilColor(50, 270, Square::white, 15);
		robot.advanceRelative(6.5, 100, 90);

		robot.setPosition(s.getRobotAlignedPosition()); //set the position
		robot.setHeading(s.getApproachHeading()); //set the heading
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
