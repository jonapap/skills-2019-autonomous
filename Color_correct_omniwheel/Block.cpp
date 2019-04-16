#include "Block.h"
#include "Functions.h"
#include "DEBUG.h"

Block::~Block() { //deconstructor. It will delete the variables bellow if they are set.
	if (numAfterPos != 0) {
		delete[] afterPosition;
	}

	if (numBeforePos != 0) {
		delete[] beforePosition;
	}
}

Block::Block(const Block &old_block) : square(old_block.square) {
	position = old_block.position;
	heading = old_block.heading;
	approachSide = old_block.approachSide;
	beforeFunction = old_block.beforeFunction;
	afterFunction = old_block.afterFunction;

	numBeforePos = old_block.numBeforePos;
	if (numBeforePos != 0) {
		beforePosition = new Position[numBeforePos];
		memcpy(beforePosition, old_block.beforePosition, sizeof(Position) * numBeforePos);
	}

	numAfterPos = old_block.numAfterPos;
	if (numAfterPos != 0) {
		afterPosition = new Position[numAfterPos];
		memcpy(afterPosition, old_block.afterPosition, sizeof(Position) * numAfterPos);
	}
}

Position Block::getLastPoint() { //return the last point the robot should go before aligning with block
	int angle = mod(mod(heading + 180, 360) + (shiftAngle * approachSide), 360);

	double deltaX = cos(toRadians(angle)) * shiftLength;
	double deltaY = sin(toRadians(angle)) * shiftLength;
	double x = position.x + deltaX;
	double y = position.y + deltaY;

	return {x,y};
}

void Block::align(OmniRobot &robot) { //align with line. No longer used
	robot.advanceUntilLine(100, approachSide == RIGHTAPPROACH ? 90 : 270);
	robot.advanceRelative(1.5, 100, 90);
	robot.goToPingDistance(100, 4);
}

double Block::getApproachHeading() { //no longer used
	return mod(heading + (90 * approachSide), 360);
}

Position Block::getRobotAlignedPosition() { //get robot's position when he is aligned with the block
	double currentHeading = mod((180 - blockGrabOffsetAngle) + heading, 360);
	return {cos(toRadians(currentHeading))*blockGrabOffsetLength+position.x, sin(toRadians(currentHeading))*blockGrabOffsetLength+position.y};
}

Square &Block::getSquare() { //return the square associated with the block
	return square;
}

void Block::setAfterPosition(Position p) { //set after position if there is only one position
	setAfterPosition(new Position[1]{p}, 1); //call the other function
}

void Block::setBeforePosition(Position p) { //set before position if there is only one position
	setBeforePosition(new Position[1]{p}, 1); //call the other function
}

void Block::setAfterPosition(Position *p, int num) { //set after positions. The pointer must point to a dynamic array
	afterPosition = p;

	numAfterPos = num;
}

void Block::setBeforePosition(Position *p, int num) { //set after positions. The pointer must point to a dynamic array
	beforePosition = p;

	numBeforePos = num;
}

//Sets the function to be used after placing block
void Block::setAfterFunction(void (*f)(Block&)){
	afterFunction = f;
}

//Call the function to be used after placing block
void Block::callAfterFunction(){
	if(afterFunction != NULL){
		afterFunction(*this);
	}
}

//Sets the function to be used after placing block
void Block::setBeforeFunction(void (*f)(Block&)){
	beforeFunction = f;
}

//Call the function to be used after placing block
void Block::callBeforeFunction(){
	if(beforeFunction != NULL){
		beforeFunction(*this);
	}
}
