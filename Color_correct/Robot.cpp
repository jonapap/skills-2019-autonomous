#include "Robot.h"
#include "DEBUG.h"
#include "Functions.h"

const RGB Robot::black = { 22, 27, 8 };
const RGB Robot::red = { 52, 13, 4 };
const RGB Robot::blue = { 10, 36, 35 };
const RGB Robot::yellow = { 71, 62, 3 };

boolean RGB::isColor(const RGB &color, int error) {
	if (inRange(color.red, red, error) && inRange(color.green, green, error)
			&& inRange(color.blue, blue, error)) {
		return true;
	}
	return false;
}

//advance distance in IN
void Robot::advanceIN(double distance, double motorSpeed) {
	double revolution = (distance / wheelcirIN) * 1440; //get how many wheel turn we need to make, and multiply by 1440 (one revolution for encoder)
	long revolution1 = getEncoderCount(1) + revolution; //add to current encoder value of motor 1. Here, the current encoder count is inverted because we inverted this motor at the beginning, but this function is not inverted by default
	long revolution2 = getEncoderCount(2) + revolution; //add to current encoder value of motor 2

	motorSpeed = (distance > 0) ? motorSpeed : -motorSpeed; //if distance is positive, robot will go forward, if negative, backwards
	prizm.setMotorTargets(motorSpeed, revolution1, motorSpeed, revolution2);

	waitForMotors();

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");

	DEBUG_PRINTLN("Heading :");
	DEBUG_PRINTLN(heading);

	DEBUG_PRINTLN("Distance :");
	DEBUG_PRINTLN(distance);

	x += cos(toRadians(heading)) * distance;
	y += sin(toRadians(heading)) * distance;

}

void Robot::rampSpeed(unsigned int speed, unsigned int time) {
	for (unsigned int i = 0; i < speed; i += speed / (time / 10)) {
		prizm.setMotorSpeeds(i, i);
	}
}

//turn specified value. Positive degrees will turn clockwise, negative anti-clockwise
void Robot::turn(double degrees, int speed) {
	double revolution = degrees * turnvalue; //calculate encoder count required to add for each wheel
	long revolution1 = (prizm.readEncoderCount(1) * motor1invert) + revolution; //add to current encoder count of motor 1
	long revolution2 = (prizm.readEncoderCount(2) * motor2invert) - revolution; //negative of second motor so robot will turn on itself

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Turning : ");
	DEBUG_PRINTLN(degrees);

	prizm.setMotorTargets(speed, revolution1, speed, revolution2); //set target of both motors
	waitForMotors(); //wait here while robot is turning

	heading = mod(heading + degrees, 360);
}

Position Robot::getPosition() {
	return {x,y};
}

double Robot::getHeading() {
	return heading;
}

void Robot::goToPosition(Position p, int speed) {
	goToPosition(p.x, p.y, speed, speed);
}

void Robot::goToPosition(Position p, int speed, int turningSpeed) {
	goToPosition(p.x, p.y, speed, turningSpeed);
}

void Robot::goToPosition(double x2, double y2, int speed) {
	goToPosition(x2, y2, speed, speed);
}

void Robot::goToPosition(double x2, double y2, int speed, int turningSpeed) {
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

	DEBUG_PRINT("Angle :");
	DEBUG_PRINTLN(angle);
	DEBUG_PRINT("Distance :");
	DEBUG_PRINTLN(distance);
	DEBUG_PRINTLN("");

	goToHeading(angle < 0 ? 360 + angle : angle, turningSpeed);
	advanceIN(distance, speed);
}

void Robot::goToHeading(double h, int speed) {
	double diff = mod(h, 360) - heading;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Diff : ");
	DEBUG_PRINTLN(diff);
	DEBUG_PRINTLN("");

	turn(diff > 180 ? -360 + diff : ((diff < -180) ? 360 + diff : diff), speed);
}

void Robot::setHeading(double h) {
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);

	heading = mod(h, 360);
	DEBUG_PRINTLN(heading);
	DEBUG_PRINTLN("");
}

void Robot::advanceUntilLine(int speed, boolean stop) { //advance (or move back if speed is negative) until he sees a line. If stop is true, robot will stop at line
	long encoder1 = getEncoderCount(1);
	long encoder2 = getEncoderCount(2);
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	prizm.setMotorSpeeds(speed, speed);

	/*while (prizm.readLineSensor(lineSensor) == 1)
	 ; //continue advancing if robot currently sees a line*/

	while (true) {
		int line = 0;
		for (int i = 0; i < 10; i++) {
			if (prizm.readLineSensor(lineSensorRight) == 1) {
				line++;
			}
		}

		if (line > 3)
			break;
	}

	if (stop) {
		prizm.setMotorSpeeds(0, 0); //stop when he sees the line
	}

	updatePosition(encoder1, encoder2);
}

void Robot::alignWithLine(int speed) { //direction == 1, turn right; direction == -1, turn left
	prizm.setMotorSpeeds(speed, speed);

	int right = 0;
	int left = 0;
	while (true) {
		delay(50);
		right = prizm.readLineSensor(lineSensorRight);
		left = prizm.readLineSensor(lineSensorLeft);

		if (right == 1 && left == 1) {
			break;
		} else if (right == 0 && left == 0) {
			prizm.setMotorSpeeds(speed, speed);
		} else if (right == 1) {
			holdMotor(2);
			prizm.setMotorSpeeds(-speed,0);
			delay(1000);
		} else if (left == 1) {
			holdMotor(2);
			prizm.setMotorSpeeds(0, -speed);
			delay(1000);
		}
	}

	prizm.setMotorSpeeds(0, 0);
}

void Robot::waitForMotors() { //when called, function will only finish when both motors are at rest.

	while (prizm.readMotorBusy(1) == 1 || prizm.readMotorBusy(2) == 1)
		//wait here while at least one motor is doing something
		;
}

void Robot::waitForEncoder(long target1, long target2) { //this function takes the target we want to go for each. Function will finish only when we are at the correct encoder count for both motors. This function can only be used if both motors go in the same direction
	signed long diff = target1 - (prizm.readEncoderCount(1) * -1); //check which side robot will go. This only check for one motor and assumes it is the same for the other.
	if (diff > 0) { //if target is greater then current count
		while (target1 >= (prizm.readEncoderCount(1) * motor1invert)
				&& target2 >= (prizm.readEncoderCount(2) * motor2invert))
			//wait here until both encoder counts are greater or equal to both targets
			;
	} else { //if target is lesser then current count
		while (target1 <= (prizm.readEncoderCount(1) * motor1invert)
				&& target2 <= (prizm.readEncoderCount(2) * motor2invert))
			//wait until both encoder counts are lesser or equal to both targets
			;
	}
}

boolean Robot::checkForEncoder(long target1, long target2, long diff) {
	if (diff > 0) { //if target is greater then current count
		if (target1 >= (prizm.readEncoderCount(1) * motor1invert)
				&& target2 >= (prizm.readEncoderCount(2) * motor2invert))
			return false; //return false if count is not greater or equal to target

	} else { //if target is lesser then current count
		if (target1 <= (prizm.readEncoderCount(1) * motor1invert)
				&& target2 <= (prizm.readEncoderCount(2) * motor2invert))
			return false; //return false if count is not lesser or equal to target
	}

	return true; //if none of the others returned false, we reached target so return true
}

void Robot::invertMotor(int motor, int invert) {
	motor1invert = motor == 1 ? (invert == 1 ? -1 : 1) : motor1invert;

	motor2invert = motor == 2 ? (invert == 1 ? -1 : 1) : motor2invert;

	prizm.setMotorInvert(motor, invert);

}

void Robot::gripperHor(int direction) {
	prizm.setServoPosition(gripperHorizontal, direction);
}

void Robot::gripperVert(int direction) {
	prizm.setServoPosition(gripperVertical, direction);
}

void Robot::goToPingDistance(int speed, int target) {
	long encoder1 = getEncoderCount(1);
	long encoder2 = getEncoderCount(2);

	double distance = 0.0;
	do {
		distance = prizm.readSonicSensorIN(pingSensor);

		if(distance > target)
			prizm.setMotorSpeeds(speed, speed);
		else if (distance < target)
			prizm.setMotorSpeeds(-speed, -speed);

	} while(distance != target);

	prizm.setMotorSpeeds(0, 0);

	updatePosition(encoder1, encoder2);
}

void Robot::setPosition(Position p) {
	setPosition(p.x, p.y);
}

void Robot::setPosition(double x, double y) {
	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Setting position :");
	DEBUG_PRINTLN(x);
	DEBUG_PRINTLN(y);

	this->x = x;
	this->y = y;
}

RGB Robot::readColor() {
	GroveColorSensor colorSensor;

	int red, green, blue;

	colorSensor.ledStatus = 1;
	colorSensor.readRGB(&red, &green, &blue);
	delay(300);
	colorSensor.clearInterrupt();

	return RGB { red, green, blue };
}

long Robot::getEncoderCount(int motor) {
	return motor == 1 ?
			(prizm.readEncoderCount(1) * motor1invert) :
			(prizm.readEncoderCount(2) * motor2invert);
}

void Robot::updatePosition(long encoder1, long encoder2) {
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

void Robot::updateAngle(long encoder1, long encoder2) {
	long revolutions = ((-getEncoderCount(1) + encoder1)
			+ (getEncoderCount(2) - encoder2)) / 2;
	double degrees = revolutions / turnvalue;

	DEBUG_PRINTLN(__PRETTY_FUNCTION__);
	DEBUG_PRINTLN("");
	DEBUG_PRINT("Angle turned : ");
	DEBUG_PRINTLN(degrees);

	heading = mod(heading + degrees, 360);
}

void Robot::holdMotor(int motor){
	prizm.setMotorTarget(motor, 5, prizm.readEncoderCount(motor));
}

void Robot::holdAllMotors(){
	prizm.setMotorTargets(5, prizm.readEncoderCount(1), 5, prizm.readEncoderCount(2));
}

