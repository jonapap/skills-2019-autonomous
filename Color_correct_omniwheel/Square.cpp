#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 246, 213, 26 };
const RGB Square::red = { 156, 11, 6 };
const RGB Square::blue = { 20, 77, 79 };
const RGB Square::yellow = { 250, 78, 6 };

Position Square::getApproachPosition() {
	int offsetHeading = mod(heading + 180, 360);

	return {cos(toRadians(offsetHeading))*approachOffset+position.x, sin(toRadians(offsetHeading))*approachOffset+position.y};
}

double Square::getApproachHeading() {
	return heading;
}

Position Square::getRobotAlignedPosition() {
	double currentHeading = mod((180 - alignedOffsetAngle) + heading, 360);
	return {cos(toRadians(currentHeading))*alignedOffsetLength+position.x, sin(toRadians(currentHeading))*alignedOffsetLength+position.y};
}

void Square::align(OmniRobot &robot) {
	robot.advanceUntilColor(50, 0, color, colorError);
	robot.advanceUntilColor(50, 270, white, 15);
	//robot.advanceUntilColor(50, 90, color, colorError);
	robot.advanceRelative(6.5, 100, 90);
}
