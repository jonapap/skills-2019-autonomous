#include "OmniRobot.h"

#include "DEBUG.h"
#include "Functions.h"
#include "Block.h"

void OmniRobot::advanceAbsolute(double distance, double motorSpeed,
		double angle) { //advance robot in specified direction and distance. The angle is relative to the entire court, independent of the robot's heading
	advanceRelative(distance, motorSpeed, getRelativeAngle(angle));
}

//advance distance in IN
void OmniRobot::advanceRelative(double distance, double motorSpeed,
		double angle) { //advance robot in specified direction and distance. The angle is relative to the robot

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	double angleOffset = mod(angle + 45, 360); //offset angle so 0 is the gripper

	double distanceX = cos(toRadians(angleOffset)) * distance; //get the distance to travel in X and Y
	double distanceY = sin(toRadians(angleOffset)) * distance;

	double revolutionX = (distanceX / wheelcirIN) * 1440; //get how many wheel turn we need to make, and multiply by 1440 (one revolution for encoder)
	double revolutionY = (distanceY / wheelcirIN) * 1440;

	//revolutionX = isZero(revolutionX) ? 5 : revolutionX; //Make sure all wheels turn a bit to prevent a possible bug in the PRIZM.
	//revolutionY = isZero(revolutionY) ? 5 : revolutionY;

	long revolution1 = getEncoderCount(1) + revolutionX; //add to current encoder value of motor 1. Here, the current encoder count is inverted because we inverted this motor at the beginning, but this function is not inverted by default
	long revolution2 = getEncoderCount(2) + revolutionY; //add to current encoder value of motor 2
	long revolution3 = getEncoderCount(3) + revolutionX;
	long revolution4 = getEncoderCount(4) + revolutionY;

	double speedX = cos(toRadians(angleOffset)) * motorSpeed; //calculate speed based on angle
	double speedY = sin(toRadians(angleOffset)) * motorSpeed;

	DEBUG_PRINTLN("Speed (uncorrected)");
	DEBUG_PRINTLN(speedX);
	DEBUG_PRINTLN(speedY);
	DEBUG_PRINTLN(isZero(speedX));
	DEBUG_PRINTLN(isZero(speedY));

	//speedX = isZero(speedX) ? 10 : speedX; //Make sure the speed is never zero.
	//speedY = isZero(speedY) ? 10 : speedY; //If it is zero, the controller will try to reach the target,
										    //but will not be able to turn the wheel (speed=0), staying stuck forever.

	if(!isZero(revolutionX) && !isZero(revolutionY)){ //check if all wheels should turn. This a workaround to a PRIZM bug.
		DEBUG_PRINTLN("Not zero");
		prizm.setMotorTargets(speedX, revolution1, speedY, revolution2); //if so, set the target and speed for all wheel
		exc.setMotorTargets(dcControllerAddr, speedX, revolution3, speedY,
				revolution4);
	} else { //else, just set the target and speed for the wheels that will turn
		DEBUG_PRINTLN("Zero");
		if(!isZero(revolutionX)){
			DEBUG_PRINTLN("Setting X targets");
			prizm.setMotorTarget(1, speedX, revolution1);
			exc.setMotorTarget(dcControllerAddr, 1, speedX, revolution3);
		}

		if(!isZero(revolutionY)){
			DEBUG_PRINTLN("Setting Y targets");
			prizm.setMotorTarget(2, speedY, revolution2);
			exc.setMotorTarget(dcControllerAddr, 2, speedY, revolution4);
		}

	}


	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Current Encoders values :");
	DEBUG_PRINTLN(getEncoderCount(1));
	DEBUG_PRINTLN(getEncoderCount(2));
	DEBUG_PRINTLN(getEncoderCount(3));
	DEBUG_PRINTLN(getEncoderCount(4));
	DEBUG_PRINTLN("Encoders target :");
	DEBUG_PRINTLN(revolution1);
	DEBUG_PRINTLN(revolution2);
	DEBUG_PRINTLN(revolution3);
	DEBUG_PRINTLN(revolution4);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Speed :");
	DEBUG_PRINTLN(speedX);
	DEBUG_PRINTLN(speedY);
	DEBUG_PRINTLN("Distance : ");
	DEBUG_PRINTLN(distance);
	DEBUG_PRINTLN("Angle : ");
	DEBUG_PRINTLN(angle);
	DEBUG_PRINTLN("Angle offset : ");
	DEBUG_PRINTLN(angleOffset);

	waitForMotors();
	DEBUG_PRINTLN("Reached target");

	x += cos(toRadians(getAbsoluteAngle(angle))) * distance; //update the robot's position
	y += sin(toRadians(getAbsoluteAngle(angle))) * distance;

}

void OmniRobot::goInRelativeDirection(double motorSpeed, double angle) { //advance robot in direction relative to the robot
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Angle :");
	DEBUG_PRINTLN(angle);
	DEBUG_PRINTLN("");

	angle = mod(angle + 45, 360);

	double speedX = cos(toRadians(angle)) * motorSpeed;
	double speedY = sin(toRadians(angle)) * motorSpeed;

	prizm.setMotorSpeeds(speedX, speedY);
	exc.setMotorSpeeds(1, speedX, speedY);

}

void OmniRobot::goInAbsoluteDirection(double motorSpeed, double angle) { //advance robot in direction relative to the court (independent of robot's heading)
	goInRelativeDirection(motorSpeed, getRelativeAngle(angle));
}

void OmniRobot::stopAllMotors() { //stop all motors
	prizm.setMotorSpeeds(0, 0);
	exc.setMotorSpeeds(1, 0, 0);
}

void OmniRobot::rampSpeed(unsigned int speed, unsigned int time) { //gradually accelerate motors. Not used and probably not working
	for (unsigned int i = 0; i < speed; i += speed / (time / 10)) {
		prizm.setMotorSpeeds(i, i);
	}
}

//turn specified value. Positive degrees will turn clockwise, negative anti-clockwise
void OmniRobot::turn(double degrees, int speed) {
	double revolution = degrees * turnvalue; //calculate encoder count required to add for each wheel

	long revolution1 = getEncoderCount(1) - revolution; //add to current encoder count of motor 1
	long revolution2 = getEncoderCount(2) - revolution; //negative of second motor so robot will turn on itself
	long revolution3 = getEncoderCount(3) + revolution;
	long revolution4 = getEncoderCount(4) + revolution;

	prizm.setMotorTargets(speed, revolution1, speed, revolution2); //set target of both motors
	exc.setMotorTargets(dcControllerAddr, speed, revolution3, speed,
			revolution4);
	waitForMotors(); //wait here while robot is turning

	heading = mod(heading + degrees, 360); //update heading
}

void OmniRobot::turnInDirection(double motorSpeed) { //turn robot in a direction
	prizm.setMotorSpeeds(-motorSpeed, -motorSpeed); //set target of both motors
	exc.setMotorSpeeds(dcControllerAddr, motorSpeed, motorSpeed);
}

Position OmniRobot::getPosition() {
	return {x,y};
}

double OmniRobot::getHeading() {
	return heading;
}

void OmniRobot::goToPosition(Position p, int speed, boolean turn) { //go to the specified position
	goToPosition(p.x, p.y, speed, turn);
}

void OmniRobot::goToPosition(double x2, double y2, int speed, boolean turn) { //go to the specified position
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Current pos : ");
	DEBUG_PRINTLN(x);
	DEBUG_PRINTLN(y);

	DEBUG_PRINTLN("Going to : ");
	DEBUG_PRINTLN(x2);
	DEBUG_PRINTLN(y2);

	double deltaX = x2 - x;
	double deltaY = y2 - y;

	DEBUG_PRINTLN("Delta x and y :");
	DEBUG_PRINTLN(deltaX);
	DEBUG_PRINTLN(deltaY);

	double angle = atan2(deltaY, deltaX) * 180 / PI;

	double distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

	DEBUG_PRINT("Angle : ");
	DEBUG_PRINTLN(angle);
	DEBUG_PRINT("Distance : ");
	DEBUG_PRINTLN(distance);
	DEBUG_PRINTLN("");

	if(turn)
		goToModHeading(angle+45, 100);

	advanceAbsolute(distance, speed, angle);
}

void OmniRobot::goToHeading(double h, int speed) { //go to the specified heading
	double diff = mod(h, 360) - heading;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Diff : ");
	DEBUG_PRINTLN(diff);
	DEBUG_PRINTLN("");

	turn(diff > 180 ? -360 + diff : ((diff < -180) ? 360 + diff : diff), speed);
}

//This function will turn the robot the quickest way where the heading is h, 2h, 3h or 4h. If h is greater then 90, h will become the reminder of h/90.
//For example, if h is 45, the robot will be turned to 45, 135, 225, or 315 depending on which is the closest.
//Another example, 180. The robot will turn to 0, 90, 180, or 270
void OmniRobot::goToModHeading(double h, int speed) {
	h = mod(h, 90); //make sure h is between 0 to 90
	double lowesth = h; //initialize this value to be h
	double lowestdiff = h - heading; //initialize this value to some valid diff

	//Loop from i = -1 to i = 4. While this loop will run 6 times, this is to have an extra run for h and 3h
	//If there is not extra run for these two, they might not get included in some instances because degrees loop back at 360
	//For example, if h is 80 and the robot's heading is 10, if there is not these extra runs, the result of the loop would be 80, but should be -10 (or 350 degrees)
	for (int i = -1; i < 5; i++) {
		double h2 = h + i * 90; //get heading to test

		double diff = h2 - heading; //calculate the difference

		if (abs(diff) < abs(lowestdiff)) { //if it is quicker with this heading
			lowesth = h2; //set this to be the lowest heading
			lowestdiff = diff; //with the lowest difference
		}
	}

	goToHeading(mod(lowesth, 360), speed); //go to the calculated heading, making sure the angle is between 0-360
}

void OmniRobot::setHeading(double h) { //updates the robot's heading
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	heading = mod(h, 360);
	DEBUG_PRINTLN(heading);
	DEBUG_PRINTLN("");
}

void OmniRobot::advanceUntilLine(int speed, double direction, boolean stop) { //advance (or move back if speed is negative) until robot sees a line. If stop is true, robot will stop at line
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	goInRelativeDirection(speed, direction);

	while (!readLineSensor(lineSensorFront))
		;

	if (stop) {
		stopAllMotors(); //stop when he sees the line
	}

}

boolean OmniRobot::advanceUntilColor(int speed, double direction, RGB color,
		int colorError, boolean invert, boolean stop, unsigned long timeout) { //advance (or move back if speed is negative) until he sees the specified color (within the color error)
	//If invert is true, the function will end when the robot stop seeing the specified color. If stop is true, robot will stop at the color.
	//timeout is how long the function will wait for reaching a square in millisecond. Set to 0 if robot should check indefinitely.
	//The function will return true if the color has been reached, false if it has stopped because of the timeout.
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	goInRelativeDirection(speed, direction); //start going in the direction

	unsigned long timeStart = millis();
	while (readColor().isColor(color, colorError) == invert
			&& (timeout == 0 ? true : millis() - timeStart < timeout))
		; //do nothing until we reached the color, or we have been here longer then the timeout

	if (stop) { //stop motors if we need to
		stopAllMotors();
	}

	if(millis()-timeStart >= timeout)
		return false; //return false if we stopped because of the timeout
	else
		return true; //return true if we reached the color
}

boolean OmniRobot::advanceUntilColor(int speed, double direction, boolean stop, unsigned long timeout) { //advance (or move back if speed is negative) until he sees a color contrast.
	//If invert is true, the function will end when the robot stop seeing the specified color. If stop is true, robot will stop at line.
	//timeout is how long the function will wait for reaching a square in millisecond. 0 if robot should check indefinitely.
	//The function will return true if the color has been reached, false if it stopped because of the timeout.
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	goInRelativeDirection(speed, direction); //move in the direction

	RGB buffer[24]; //create buffer for holding past color values. Robot reads 24 color / second, so buffer will overwrite each second

	int samecolorcount = 0;
	RGB color1 = readColor();
	while(samecolorcount < 4){ //while we don't see the same color 7 times
		RGB color2 = readColor(); //read current color

		if(color1.isColor(color2, 20)){ //if the two color are the same
			samecolorcount++; //increase the counter
		} else {
			color1 = color2; //if not, change the color
			samecolorcount = 0; //reset counter
		}
	}

	sendln("Color reference (RGB) : ", color1.red, ", ", color1.green, ", ", color1.blue);


	for(int i = 0; i < 24; i++){
		buffer[i] = color1; //initialize the array to present color
	}

	int hitcount = 0;
	int buffercount = 0;
	unsigned long timeStart = millis();
	while (timeout == 0 ? true : millis() - timeStart < timeout) {
		RGB color = readColor();
		RGB &pastcolor = buffer[buffercount];

		//calculate the difference between current color and the color from a second ago.
		int diff = (abs(color.red - pastcolor.red)
				+ abs(color.green - pastcolor.green)
				+ abs(color.blue - pastcolor.blue)) / 3;

		if (diff > 35) {//if diff is greater then 35
			DEBUG_PRINT("Diff : ");
			DEBUG_PRINTLN(diff);
			hitcount++; //add one to hitcount
			timeStart = millis(); //reset the timer so we don't timeout too soon
		}

		if(hitcount == 10) { //if we saw a contrast 10 times, get out of loop
			break;
		}

		buffer[buffercount] = color; //update the buffer
		buffercount = buffercount == 23 ? 0 : buffercount + 1; //if count equals 23, get back to 0, else add one to buffercount
	}

	DEBUG_PRINTLN("");

	if (stop) {
		stopAllMotors(); //stop if needed
	}

	if(millis()-timeStart >= timeout)
		return false; //return false if we stopped because of the timeout
	else
		return true; //return true if we reached the color
}

void OmniRobot::alignWithPing() { //function to aligning using the two ping. No longer used
	int speed = 25;

	double diff;

	do {
		double pingRight = prizm.readSonicSensorIN(pingSensorLeft);
		delay(10);
		double pingLeft = prizm.readSonicSensorIN(pingSensorRight);

		diff = pingRight-pingLeft;

		if (pingRight > pingLeft) {
			turnInDirection(-speed);
		} else if (pingLeft > pingRight) {
			turnInDirection(speed);
		}

	} while (abs(diff) > 0.05);

	stopAllMotors();
}

void OmniRobot::waitForMotors() { //when called, function will only finish when all motors are at rest.

	while (prizm.readMotorBusy(1) == 1 || prizm.readMotorBusy(2) == 1
			|| exc.readMotorBusy(dcControllerAddr, 1) == 1
			|| exc.readMotorBusy(dcControllerAddr, 2) == 1)
		//wait here while at least one motor is doing something
		;
}

void OmniRobot::waitForEncoder(long target1, long target2) { //Not used. this function takes the target we want to go for each. Function will finish only when we are at the correct encoder count for both motors. This function can only be used if both motors go in the same direction
	signed long diff = target1 - (prizm.readEncoderCount(1) * -1); //check which side robot will go. This only check for one motor and assumes it is the same for the other.
	if (diff > 0) { //if target is greater then current count
		while (target1 >= (prizm.readEncoderCount(1) * getMotorInvert(1))
				&& target2 >= (prizm.readEncoderCount(2) * getMotorInvert(2)))
			//wait here until both encoder counts are greater or equal to both targets
			;
	} else { //if target is lesser then current count
		while (target1 <= (prizm.readEncoderCount(1) * getMotorInvert(1))
				&& target2 <= (prizm.readEncoderCount(2) * getMotorInvert(2)))
			//wait until both encoder counts are lesser or equal to both targets
			;
	}
}

boolean OmniRobot::checkForEncoder(long target1, long target2, long diff) { //Not used.
	if (diff > 0) { //if target is greater then current count
		if (target1 >= (prizm.readEncoderCount(1) * getMotorInvert(1))
				&& target2 >= (prizm.readEncoderCount(2) * getMotorInvert(2)))
			return false; //return false if count is not greater or equal to target

	} else { //if target is lesser then current count
		if (target1 <= (prizm.readEncoderCount(1) * getMotorInvert(1))
				&& target2 <= (prizm.readEncoderCount(2) * getMotorInvert(2)))
			return false; //return false if count is not lesser or equal to target
	}

	return true; //if none of the others returned false, we reached target so return true
}

//Invert the motor's direction. Use this instead of prizm's function so this class will have the correct encoders values.
void OmniRobot::invertMotor(int motor, int invert) {
	motorinvert[motor - 1] = invert == 1 ? -1 : 1;

	switch (motor) {
	case 1:
	case 2:
		prizm.setMotorInvert(motor, invert);
		break;
	case 3:
	case 4:
		exc.setMotorInvert(dcControllerAddr, motor - 2, invert);
		break;
	}

}

void OmniRobot::gripperHor(int direction) { //open or close gripper
	prizm.setServoPosition(gripperHorizontal, direction);

}

void OmniRobot::gripperVert(int direction) {//move gripper up or down
	prizm.setServoPosition(gripperVertical, direction);

}

double OmniRobot::readPing(int pin){ //read ping in inches
	double dist = prizm.readSonicSensorIN(pin);
	delay(10);
	return dist;
}

//go to specified distance (target) using ping
//if back is false, the robot will only go forward. If true, it will go forward or backward to reach the target
void OmniRobot::goToPingDistance(int speed, double target, int back) {
	if(back == false){
		goInRelativeDirection(speed, 0);

		while (readPing(pingSensorRight) > target && readPing(pingSensorLeft) > target);

		stopAllMotors();
	} else {
		int greater = readPing(pingSensorRight) > target
				|| readPing(pingSensorLeft) > target;
		goInRelativeDirection(speed, greater ? 0 : 180);

		while (greater ?
				(readPing(pingSensorRight) > target && readPing(pingSensorLeft) > target) :
				(readPing(pingSensorRight) < target && readPing(pingSensorLeft) < target)) ;

		stopAllMotors();
	}
}

void OmniRobot::setPosition(Position p) { //set position
	setPosition(p.x, p.y);
}

void OmniRobot::setPosition(double x, double y) { //set position
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Setting position :");
	DEBUG_PRINTLN(x);
	DEBUG_PRINTLN(y);

	this->x = x;
	this->y = y;
}

RGB OmniRobot::readColor() { //read color sensor
	GroveColorSensor colorSensor;

	int red, green, blue;

	colorSensor.ledStatus = 1;
	colorSensor.readRGB(&red, &green, &blue);
	//delay(300);
	colorSensor.clearInterrupt();

	return RGB { red, green, blue };
}

//return the encoder count for the motor (value between 1 to 4)
//this takes into account if the motor is inverted
long OmniRobot::getEncoderCount(int motor) {
	switch (motor) {
	case 1:
	case 2:
		return prizm.readEncoderCount(motor) * getMotorInvert(motor);
		break;
	case 3:
	case 4:
		return exc.readEncoderCount(dcControllerAddr, motor - 2)
				* getMotorInvert(motor);
		break;
	}
	return 0;
}

//No longer used. Calculate the distance the robot has moved based on past encoder values, and updates his position
void OmniRobot::updatePosition(long encoder1, long encoder2) {
	long revolutions = ((getEncoderCount(1) - encoder1)
			+ (getEncoderCount(2) - encoder2)) / 2; //take the average of the two encoders
	double distance = (revolutions / 1440.0) * wheelcirIN;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Distance : ");
	DEBUG_PRINTLN(distance);

	x += cos(heading) * distance;
	y += sin(heading) * distance;
}

//No longer used. Update his heading based on past encoder values
void OmniRobot::updateAngle(long encoder1, long encoder2) {
	long revolutions = ((-getEncoderCount(1) + encoder1)
			+ (getEncoderCount(2) - encoder2)) / 2;
	double degrees = revolutions / turnvalue;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Angle turned : ");
	DEBUG_PRINTLN(degrees);

	heading = mod(heading + degrees, 360);
}

//Hold motor in place. Not used
void OmniRobot::holdMotor(int motor) {
	prizm.setMotorTarget(motor, 5, prizm.readEncoderCount(motor));
}

//Hold all motors in place. Not used
void OmniRobot::holdAllMotors() {
	prizm.setMotorTargets(5, prizm.readEncoderCount(1), 5,
			prizm.readEncoderCount(2));
}

//Return if a line sensor sees a line. This function will return true only if it gets 3 hits on a total of 10 (to avoid false positives)
boolean OmniRobot::readLineSensor(int sensor) {
	int line = 0;
	for (int i = 0; i < 10; i++) {
		if (prizm.readLineSensor(sensor) == 1) {
			line++;
		}
	}

	if (line > 3)
		return true;
	else
		return false;
}

//Get if a motor is inverted
int OmniRobot::getMotorInvert(int motor) {
	return motorinvert[motor - 1];
}

//From an absolute angle (relative to the court), get angle relative to robot
double OmniRobot::getRelativeAngle(double angle) {
	return mod(angle - heading, 360);
}

//From an angle relative to robot, get an absolute angle (relative to the court)
double OmniRobot::getAbsoluteAngle(double angle) {
	return mod(angle + heading, 360);
}

//Returns the current encoder values
EncoderValues OmniRobot::getEncoderValues(){
	return {getEncoderCount(1), getEncoderCount(2), getEncoderCount(3), getEncoderCount(4)};
}

//Go to the position when the encoder values were taken. The robot should not have turned.
void OmniRobot::goToPosition(EncoderValues values, int speed) {
	prizm.setMotorTargets(speed, values.enc1, speed, values.enc2); //return to original position
	exc.setMotorTargets(1, speed, values.enc3, speed, values.enc4);
	waitForMotors();
}

//Get the distance and angle traveled based on past encoder values
Vector OmniRobot::getDistance(EncoderValues values) {
		double revolutionX = (getEncoderCount(1) + getEncoderCount(3))/2 - (values.enc1 + values.enc3)/2;
		double revolutionY = (getEncoderCount(2) + getEncoderCount(4))/2 - (values.enc2 + values.enc4)/2;

		double distanceX = (revolutionX/1440)*wheelcirIN;
		double distanceY = (revolutionY/1440)*wheelcirIN;

		double distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		double angle = mod(toDegrees(atan2(distanceY, distanceX))-45, 360);

		return {distance, angle};
}

//turn robot until he sees the color. Refer to comments on advanceUntilColor() for more info.
void OmniRobot::turnUntilColor(int speed, RGB color, int colorError,
		boolean invert, boolean stop) {
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	turnInDirection(speed);

	while (readColor().isColor(color, colorError) == invert)
		;;

	if (stop) {
		stopAllMotors(); //stop when he sees the line
	}
}
