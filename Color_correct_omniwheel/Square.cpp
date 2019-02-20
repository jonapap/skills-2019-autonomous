#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 243, 214, 26 };
const RGB Square::red = { 150, 13, 7 };
const RGB Square::blue = { 22, 82, 83 };
const RGB Square::yellow = { 246, 199, 6 };

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

RGB Square::getColor(){
	return color;
}

int Square::getColorError(){
	return colorError;
}
