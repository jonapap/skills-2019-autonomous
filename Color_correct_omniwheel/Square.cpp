#include "Square.h"
#include "Functions.h"

const RGB Square::white = { 139, 122, 38 }; //define values for the colors
const RGB Square::red = { 78, 6, 4 };
const RGB Square::blue = { 10, 12, 41 };
const RGB Square::yellow = { 166, 52, 5 };

//Returns the position the robot should go at before aligning with square
Position Square::getApproachPosition() {
	int offsetHeading = mod(heading + 180, 360);

	return {cos(toRadians(offsetHeading))*approachOffset+position.x, sin(toRadians(offsetHeading))*approachOffset+position.y};
}

//Returns the heading the robot should be at when aligning with square
double Square::getApproachHeading() {
	return heading;
}

//Returns the position the robot is at when aligned with square
Position Square::getRobotAlignedPosition() {
	double currentHeading = mod((180 - alignedOffsetAngle) + heading, 360);
	return {cos(toRadians(currentHeading))*alignedOffsetLength+position.x, sin(toRadians(currentHeading))*alignedOffsetLength+position.y};
}

//Returns the square's color
RGB Square::getColor(){
	return color;
}

//Returns the color error to be used when searching for square
int Square::getColorError(){
	return colorError;
}

//Sets the function to be used after placing block
void Square::setAfterFunction(void (*f)(Square&)){
	afterFunction = f;
}

//Call the function to be used after placing block
void Square::callAfterFunction(){
	if(afterFunction != NULL){
		afterFunction(*this);
	}
}

//Sets the function to be used after placing block
void Square::setBeforeFunction(void (*f)(Square&)){
	beforeFunction = f;
}

//Call the function to be used after placing block
void Square::callBeforeFunction(){
	if(beforeFunction != NULL){
		beforeFunction(*this);
	}
}
