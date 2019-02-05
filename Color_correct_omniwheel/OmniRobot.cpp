#include "OmniRobot.h"

#include "DEBUG.h"
#include "Functions.h"
#include "Block.h"

boolean RGB::isColor(const RGB &color, int error) {
	if (inRange(color.red, red, error) && inRange(color.green, green, error)
			&& inRange(color.blue, blue, error)) {
		return true;
	}
	return false;
}

void OmniRobot::advanceAbsolute(double distance, double motorSpeed,
		double angle) {
	advanceRelative(distance, motorSpeed, getRelativeAngle(angle));
}

//advance distance in IN
void OmniRobot::advanceRelative(double distance, double motorSpeed,
		double angle) {

	double angleOffset = mod(angle + 45, 360); //offset angle so 0 is the gripper

	double distanceX = cos(toRadians(angleOffset)) * distance;
	double distanceY = sin(toRadians(angleOffset)) * distance;

	double revolutionX = (distanceX / wheelcirIN) * 1440; //get how many wheel turn we need to make, and multiply by 1440 (one revolution for encoder)
	double revolutionY = (distanceY / wheelcirIN) * 1440;

	long revolution1 = getEncoderCount(1) + revolutionX; //add to current encoder value of motor 1. Here, the current encoder count is inverted because we inverted this motor at the beginning, but this function is not inverted by default
	long revolution2 = getEncoderCount(2) + revolutionY; //add to current encoder value of motor 2
	long revolution3 = getEncoderCount(3) + revolutionX;
	long revolution4 = getEncoderCount(4) + revolutionY;

	double speedX = cos(toRadians(angleOffset)) * motorSpeed;
	double speedY = sin(toRadians(angleOffset)) * motorSpeed;

	prizm.setMotorTargets(speedX, revolution1, speedY, revolution2);
	exc.setMotorTargets(dcControllerAddr, speedX, revolution3, speedY,
			revolution4);

	waitForMotors();

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Encoders target :");
	DEBUG_PRINTLN(revolution1);
	DEBUG_PRINTLN(revolution2);
	DEBUG_PRINTLN(revolution3);
	DEBUG_PRINTLN(revolution4);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Distance : ");
	DEBUG_PRINTLN(distance);

	DEBUG_PRINTLN("Angle : ");
	DEBUG_PRINTLN(angle);

	x += cos(toRadians(getAbsoluteAngle(angle))) * distance;
	y += sin(toRadians(getAbsoluteAngle(angle))) * distance;

}

void OmniRobot::goInRelativeDirection(double motorSpeed, double angle) {
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

void OmniRobot::goInAbsoluteDirection(double motorSpeed, double angle) {
	goInRelativeDirection(motorSpeed, getRelativeAngle(angle));
}

void OmniRobot::stopAllMotors() {
	prizm.setMotorSpeeds(0, 0);
	exc.setMotorSpeeds(1, 0, 0);
}

void OmniRobot::rampSpeed(unsigned int speed, unsigned int time) {
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

	heading = mod(heading + degrees, 360);
}

void OmniRobot::turnInDirection(double motorSpeed) {
	prizm.setMotorSpeeds(motorSpeed, motorSpeed); //set target of both motors
	exc.setMotorSpeeds(dcControllerAddr, -motorSpeed, -motorSpeed);
}

Position OmniRobot::getPosition() {
	return {x,y};
}

double OmniRobot::getHeading() {
	return heading;
}

void OmniRobot::goToPosition(Position p, int speed) {
	goToPosition(p.x, p.y, speed);
}

void OmniRobot::goToPosition(double x2, double y2, int speed) {
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

	advanceAbsolute(distance, speed, angle);
}

void OmniRobot::goToHeading(double h, int speed) {
	double diff = mod(h, 360) - heading;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Diff : ");
	DEBUG_PRINTLN(diff);
	DEBUG_PRINTLN("");

	turn(diff > 180 ? -360 + diff : ((diff < -180) ? 360 + diff : diff), speed);
}

void OmniRobot::setHeading(double h) {
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	heading = mod(h, 360);
	DEBUG_PRINTLN(heading);
	DEBUG_PRINTLN("");
}

void OmniRobot::advanceUntilLine(int speed, double direction, boolean stop) { //advance (or move back if speed is negative) until he sees a line. If stop is true, robot will stop at line
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	goInRelativeDirection(speed, direction);

	while (!readLineSensor(lineSensorFront))
		;

	if (stop) {
		stopAllMotors(); //stop when he sees the line
	}

}

void OmniRobot::advanceUntilColor(int speed, double direction, RGB color,
		int colorError, boolean invert, boolean stop) { //advance (or move back if speed is negative) until he sees a line. If stop is true, robot will stop at line
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	goInRelativeDirection(speed, direction);

	while (readColor().isColor(color, colorError) == invert)
		;;

	if (stop) {
		stopAllMotors(); //stop when he sees the line
	}

}

void OmniRobot::alignWithPing() {
	int speed = 25;

	double diff;

	do {
		double pingRight = prizm.readSonicSensorIN(pingSensorLeft);
		delay(10);
		double pingLeft = prizm.readSonicSensorIN(pingSensorRight);

		diff = pingRight-pingLeft;

		if (pingRight > pingLeft) {
			turnInDirection(speed);
		} else if (pingLeft > pingRight) {
			turnInDirection(-speed);
		}

	} while (abs(diff) > 0.05);

	stopAllMotors();
}

void OmniRobot::waitForMotors() { //when called, function will only finish when both motors are at rest.

	while (prizm.readMotorBusy(1) == 1 || prizm.readMotorBusy(2) == 1
			|| exc.readMotorBusy(dcControllerAddr, 1) == 1
			|| exc.readMotorBusy(dcControllerAddr, 2) == 1)
		//wait here while at least one motor is doing something
		;
}

void OmniRobot::waitForEncoder(long target1, long target2) { //this function takes the target we want to go for each. Function will finish only when we are at the correct encoder count for both motors. This function can only be used if both motors go in the same direction
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

boolean OmniRobot::checkForEncoder(long target1, long target2, long diff) {
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

void OmniRobot::gripperHor(int direction) {
	prizm.setServoPosition(gripperHorizontal, direction);

}

void OmniRobot::gripperVert(int direction) {
	prizm.setServoPosition(gripperVertical, direction);

}

void OmniRobot::goToPingDistance(int speed, int target, int back) {
	if(back == false){
		goInRelativeDirection(speed, 0);

		while (prizm.readSonicSensorIN(pingSensorRight) > target && prizm.readSonicSensorIN(pingSensorLeft) > target) {
			delay(10);
		}

		stopAllMotors();
	} else {
		int greater = prizm.readSonicSensorIN(pingSensorRight) > target;
		goInRelativeDirection(speed, greater ? 0 : 180);

		while (greater ? prizm.readSonicSensorIN(pingSensorRight) > target : prizm.readSonicSensorIN(pingSensorRight) < target) {
			delay(10);
		}

		stopAllMotors();
	}
}

void OmniRobot::setPosition(Position p) {
	setPosition(p.x, p.y);
}

void OmniRobot::setPosition(double x, double y) {
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Setting position :");
	DEBUG_PRINTLN(x);
	DEBUG_PRINTLN(y);

	this->x = x;
	this->y = y;
}

RGB OmniRobot::readColor() {
	GroveColorSensor colorSensor;

	int red, green, blue;

	colorSensor.ledStatus = 1;
	colorSensor.readRGB(&red, &green, &blue);
	//delay(300);
	colorSensor.clearInterrupt();

	return RGB { red, green, blue };
}

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

void OmniRobot::holdMotor(int motor) {
	prizm.setMotorTarget(motor, 5, prizm.readEncoderCount(motor));
}

void OmniRobot::holdAllMotors() {
	prizm.setMotorTargets(5, prizm.readEncoderCount(1), 5,
			prizm.readEncoderCount(2));
}

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

int OmniRobot::getMotorInvert(int motor) {
	return motorinvert[motor - 1];
}

double OmniRobot::getRelativeAngle(double angle) {
	return mod(angle - heading, 360);
}

double OmniRobot::getAbsoluteAngle(double angle) {
	return mod(angle + heading, 360);
}

EncoderValues OmniRobot::getEncoderValues(){
	return {getEncoderCount(1), getEncoderCount(2), getEncoderCount(3), getEncoderCount(4)};
}

void OmniRobot::goToPosition(EncoderValues values) {
	prizm.setMotorTargets(100, values.enc1, 100, values.enc2); //return to original position
	exc.setMotorTargets(1, 100, values.enc3, 100, values.enc4);
	waitForMotors();
}

Vector OmniRobot::getDistance(EncoderValues values) {
		double revolutionX = (getEncoderCount(1) + getEncoderCount(3))/2 - (values.enc1 + values.enc3)/2;
		double revolutionY = (getEncoderCount(2) + getEncoderCount(4))/2 - (values.enc2 + values.enc4)/2;

		double distanceX = (revolutionX/1440)*wheelcirIN;
		double distanceY = (revolutionY/1440)*wheelcirIN;

		double distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));
		double angle = mod(toDegrees(atan2(distanceY, distanceX))-45, 360);

		return {distance, angle};
}
