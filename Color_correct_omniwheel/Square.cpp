#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 190, 162, 20 };
const RGB Square::red = { 122, 9, 5 };
const RGB Square::blue = { 16, 59, 59 };
const RGB Square::yellow = { 221, 68, 5 };

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
	robot.advanceUntilColor(50, 270, white, 50);
	//robot.advanceUntilColor(50, 90, color, colorError);
	robot.advanceRelative(6.5, 100, 90);
}
