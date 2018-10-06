#include "Robot.h"
#include "DEBUG.h"

#define toRadians(x) x*(PI/180)
#define toDegrees(x) x/(PI/180)
#define IN_RANGE(x, y, r) abs(x-y) <= r
#define mod(x,y) (x)-y*floor((x)/y)

const RGB Robot::black = { 22, 27, 8 };
const RGB Robot::red = { 52, 13, 4 };
const RGB Robot::blue = { 10, 36, 35 };
const RGB Robot::yellow = { 71, 62, 3 };

boolean RGB::isColor(const RGB &color, int error) {
	if (IN_RANGE(color.red, red,
			error) && IN_RANGE(color.green, green, error) && IN_RANGE(color.blue, blue, error)) {
		return true;
	}
	return false;
}

//advance distance in IN
void Robot::advanceIN(double distance, double motorSpeed, boolean stop) {
	double revolution = (distance / wheelcirIN) * 1440; //get how many wheel turn we need to make, and multiply by 1440 (one revolution for encoder)
	long revolution1 = getEncoderCount(1) + revolution; //add to current encoder value of motor 1. Here, the current encoder count is inverted because we inverted this motor at the beginning, but this function is not inverted by default
	long revolution2 = getEncoderCount(2) + revolution; //add to current encoder value of motor 2

	motorSpeed = (distance > 0) ? motorSpeed : -motorSpeed; //if distance is positive, robot will go forward, if negative, backwards
	prizm.setMotorSpeeds(motorSpeed, motorSpeed); //set speed of robot
	waitForEncoder(revolution1, revolution2); //wait until both motors reach specified number of revolutions
	if (stop) {
		prizm.setMotorPowers(125, 125); //if variable is true, stop motors
	}

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
	long revolution1 = (prizm.readEncoderCount(1) * motor1invert) - revolution; //add to current encoder count of motor 1
	long revolution2 = (prizm.readEncoderCount(2) * motor2invert) + revolution; //negative of second motor so robot will turn on itself

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
	goToPosition(p.x, p.y, speed);
}

void Robot::goToPosition(double x2, double y2, int speed) {
	goToPosition(x2, y2, speed, speed);
}

void Robot::goToPosition(double x2, double y2, int speed, int turningSpeed) {
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
	double diff = h - heading;

	DEBUG_PRINT("Diff : ");
	DEBUG_PRINTLN(diff);
	DEBUG_PRINTLN("");

	turn(diff > 180 ? -360 + diff : ((diff < -180) ? 360 + diff : diff), speed);
}

void Robot::setHeading(double h) {
	heading = h;
}

void Robot::advanceUntilLine(int speed, boolean stop) { //advance (or move back if speed is negative) until he sees a line. If stop is true, robot will stop at line
	long encoder1 = getEncoderCount(1);
	long encoder2 = getEncoderCount(2);

	prizm.setMotorSpeeds(speed, speed);

	while (prizm.readLineSensor(lineSensor) == 1)
		; //continue advancing if robot currently sees a line

	while (prizm.readLineSensor(lineSensor) == 0)
		; //wait until robot see something

	if (stop) {
		prizm.setMotorSpeeds(0, 0); //stop when he sees the line
	}

	updatePosition(encoder1, encoder2);
}

void Robot::alignWithLine(int speed, int direction) { //direction == 1, turn right; direction == -1, turn left
	long encoder1 = getEncoderCount(1);
	long encoder2 = getEncoderCount(2);

	prizm.setMotorSpeeds(speed * -direction, speed * direction);
	while (prizm.readLineSensor(lineSensor) == 0)
		;

	prizm.setMotorSpeeds(0, 0);
	updateAngle(encoder1, encoder2);
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

void Robot::gripperOpen(int direction) {
	prizm.setServoPosition(gripperHorizontal, direction);
}

void Robot::gripperUp(int direction) {
	prizm.setServoPosition(gripperVertical, direction);
}

void Robot::advanceUntilPing(int speed, int distance) {
	long encoder1 = getEncoderCount(1);
	long encoder2 = getEncoderCount(2);

	prizm.setMotorSpeeds(speed, speed);

	while (prizm.readSonicSensorIN(pingSensor) > distance) {
		delay(50);
	}

	prizm.setMotorSpeeds(0, 0);

	updatePosition(encoder1, encoder2);
}

void Robot::setPosition(Position p) {
	setPosition(p.x,p.y);
}

void Robot::setPosition(double x, double y) {
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

	DEBUG_PRINT("Distance : ");
	DEBUG_PRINTLN(distance);

	x += cos(heading) * distance;
	y += sin(heading) * distance;
}

void Robot::updateAngle(long encoder1, long encoder2) {
	long revolutions = ((-getEncoderCount(1) + encoder1)
			+ (getEncoderCount(2) - encoder2)) / 2;
	double degrees = revolutions / turnvalue;

	DEBUG_PRINT("Angle turned : ");
	DEBUG_PRINTLN(degrees);

	heading = mod(heading + degrees, 360);
}

