#include "Color_corect.h"

#include "Layouts/Layout5.h"

PRIZM prizm;
EXPANSION exc;

const unsigned int startBlock = 0; //Value from 0 to 5. This will be the block where the robot starts
								   //If this is 0, robot will start at black square, any other values, will start right next to the specified block

void setup() {

#ifdef DEBUG
	Serial.begin(9600); //Start serial if DEBUG is defined
#endif

	setupCourt();

	prizm.PrizmBegin();
	robot.invertMotor(1, 1);
	robot.invertMotor(2, 1);


	long timeStart = millis(); //get the time we are starting
	cycleBlocks();

	if(millis()-timeStart > 600000){ //if it took more then 10 min
		while(true){ //flash the red light
			prizm.setRedLED(HIGH);
			delay(500);
			prizm.setRedLED(LOW);
			delay(500);
		}
	} else { //else, end the program
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
	 Serial.println("");
	 delay(1000);*/
}

void cycleBlocks() {
	int firstTime = true;
	for (unsigned int i = startBlock; i<(sizeof(blocks)/sizeof(blocks[0])); i++) {
		Block &b = blocks[i];

		robot.gripperVert(UP);
		robot.gripperHor(OPEN);
		if (!firstTime || i == 0) { //this loop is to make it easier to start at another block then the first
									//if we specified another block else then the first one to start at,
									//this condition will skip the part where the robot goes to that block
			goToBlock(b);
			robot.goToHeading(b.heading, 100);
		}

		alignWithLine(b.approachSide);

		grabBlock();

		robot.goToPingDistance(100, 4, true);

		robot.setPosition(b.getRobotAlignedPosition()); //update robot's position and heading
		robot.setHeading(b.heading);

		robot.advanceRelative(4, 100, 180);

		Square &s = b.getSquare();
		goToSquare(b);
		robot.goToHeading(s.getApproachHeading(), 100);

		alignWithSquare(s);
		robot.setPosition(s.getRobotAlignedPosition()); //update robot's position

		depositBlock(s);

		firstTime = false;
	}
}

void goToBlock(Block &b){
	for(int i = 0; i<b.numBeforePos; i++){ //check if there is any other positions to go before the block
			robot.goToPosition(b.beforePosition[i], 200);
	}

	robot.goToPosition(b.getLastPoint(), 200); //go to the block
}

void goToSquare(Block &b){
	for(int i = 0; i<b.numAfterPos; i++){ //check if there is any other position to go after the block
		robot.goToPosition(b.afterPosition[i], 200);
	}

	robot.goToPosition(b.getSquare().getApproachPosition(), 200); //go to square
}

void depositBlock(Square &s){
	if(s.numOfBlocks == 0){ //if it is the first block
		robot.advanceRelative(3, 100, 0);
		robot.gripperVert(DOWN);
		delay(4000);
		robot.gripperHor(OPEN);
		delay(GRIPPER_TIME);
		robot.advanceRelative(5, 100, 180);
	} else { //second block
		robot.advanceRelative(3, 100, 180);
		robot.gripperVert(DOWN);
		delay(4000);
		robot.advanceRelative(3.75, 100, 0);
		robot.gripperHor(OPEN);
		delay(GRIPPER_TIME);
		robot.advanceRelative(5, 100, 180);
	}

	s.numOfBlocks++; //add one to number of blocks deposited

	s.callAfterFunction(); //call the function to execute after placing block (if there is any)
}

void grabBlock() {
	//robot.advanceRelative(3, 100, 0);
	robot.goToPingDistance(50, 0.5);

	robot.gripperVert(DOWN);
	delay(500);
	robot.gripperVert(NEUTRAL);

	robot.gripperHor(CLOSE);
	delay(GRIPPER_TIME);

	robot.gripperVert(UP);

	robot.advanceRelative(6, 100, 180);
}

void alignWithLine(int side) {
	EncoderValues encvals = robot.getEncoderValues(); //store current robot's position

	robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 90 : 270); //go in direction of line

	unsigned long timeStart = millis(); //get the current time
	while (robot.readLineSensor(robot.lineSensorFront) == 0) { //while the front sensor didin't reach a line
		if (millis() - timeStart > 10000) { //check if it has been more then 10 s
			robot.goToPosition(encvals, 100); //if yes, go to original position

			robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 270 : 90); //following code will check for 5 s if line is at the left of the robot
			unsigned long timeStart2 = millis();
			while (robot.readLineSensor(robot.lineSensorFront) == 0
					&& millis() - timeStart2 < 5000)
				;

			if (robot.readLineSensor(robot.lineSensorFront) == 1) //if the line is indeed there, get out of the loop
				break;

			robot.goToPosition(encvals, 100); //else, go to original position

			robot.advanceRelative(4, 100, 0); //advance a bit forward

			encvals = robot.getEncoderValues(); //save position

			robot.goInRelativeDirection(50, side == RIGHTAPPROACH ? 90 : 270); //start searching again for the line
			timeStart = millis(); //reset time
		}
	}

	robot.goToPingDistance(50, 5); //when we are at line, advance until the robot is at 5 in of block

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
		reached = robot.advanceUntilColor(50, 0,
				false, 8000); //try to reach square color with a 8000 ms timeout

		if (reached == false && (count == 0 || count == 1)) { //if we didn't hit the block and we are at our first or second try
			robot.goToPosition(val, 100); //return to original position
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
		/*robot.advanceRelative(2, 50, 0); //the following four lines make sure the robot is at a certain distance from the bottom and right side of the square
		robot.advanceUntilColor(50, 270, Square::white);
		robot.advanceRelative(4, 50, 90);
		robot.advanceUntilColor(50, 180, Square::white);

		int offset = 2; //how much the robot will advance from bottom of square

		robot.advanceRelative(offset, 50, 0); //advance the specified offset
		robot.turnUntilColor(50, Square::white); //turn until the color sees white

		int angle = mod(toDegrees(atan2(robot.robotradiusIN, robot.robotradiusIN - offset)),
				360); //calculate angle required base on offset and robot radius

		robot.turn(-(angle-3), 50); //turn the calculated amount (with a small offset)

		robot.advanceUntilColor(50, 180, Square::white); //following four lines will place robot at the bottom center of the robot
		robot.advanceUntilColor(50, 0, s.getColor(), s.getColorError());*/


		//this loop make sure we are the end of the square, and not at the bottom
		/*RGB pastcolor = robot.readColor(); //read current color

		while (robot.readColor().isColor(pastcolor, 20)) { //while the color didin't change
			robot.advanceUntilColor(50, 270); //advance until we reach a contrast
			robot.advanceRelative(1, 50, 315); //advance a bit to check if it is still the same color
		}

		robot.advanceRelative(1, 50, 135);*/

		/*if(robot.readColor().isColor(pastcolor, 20)) { //if we are still on square
			robot.advanceUntilColor(50, 180);
			robot.advanceUntilColor(50, 0, pastcolor);
		 }*/

		robot.advanceRelative(0.75, 50, 0);
		robot.advanceUntilColor(50, 270);

		robot.advanceRelative(7, 100, 90);

		robot.advanceUntilColor(50, 180);

		robot.advanceRelative(1, 100, 0);

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
