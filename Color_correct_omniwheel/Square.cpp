#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 139, 122, 38 };
const RGB Square::red = { 78, 6, 4 };
const RGB Square::blue = { 10, 12, 41 };
const RGB Square::yellow = { 166, 52, 5 };

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

void Square::setAfterFunction(void (*f)(Square&)){
	afterFunction = f;
}

void Square::callAfterFunction(){
	if(afterFunction != NULL){
		afterFunction(*this);
	}
}
