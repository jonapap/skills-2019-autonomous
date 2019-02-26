#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 222, 197, 24 };
const RGB Square::red = { 140, 11, 6 };
const RGB Square::blue = { 20, 75, 72 };
const RGB Square::yellow = { 253, 204, 6 };

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
