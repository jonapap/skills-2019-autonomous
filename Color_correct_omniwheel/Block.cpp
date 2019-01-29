#include "Block.h"
#include "Functions.h"
#include "DEBUG.h"

Block::~Block(){
	if(numAfterPos != 0){
		delete []afterPosition;
	}

	if(numBeforePos != 0){
			delete []beforePosition;
		}
}

Position Block::getLastPoint() {
	int angle = mod(mod(heading + 180, 360) + (shiftAngle * approachSide), 360);

	double deltaX = cos(toRadians(angle)) * shiftLength;
	double deltaY = sin(toRadians(angle)) * shiftLength;
	double x = position.x + deltaX;
	double y = position.y + deltaY;

	return {x,y};
}

void Block::align(OmniRobot &robot) {
	robot.advanceUntilLine(100, approachSide == RIGHTAPPROACH ? 90 : 270);
	robot.advanceRelative(1.5, 100, 90);
	robot.goToPingDistance(100, 4);
}

double Block::getApproachHeading() {
	return mod(heading + (90 * approachSide), 360);
}

Position Block::getRobotAlignedPosition() {
	double currentHeading = mod((180 - blockGrabOffsetAngle) + heading, 360);
	return {cos(toRadians(currentHeading))*blockGrabOffsetLength+position.x, sin(toRadians(currentHeading))*blockGrabOffsetLength+position.y};
}

Square &Block::getSquare() {
	return square;
}

void Block::setAfterPosition(Position p) {
	setAfterPosition(new Position[1]{p}, 1);
}

void Block::setBeforePosition(Position p) {
	setBeforePosition(new Position[1]{p}, 1);
}

void Block::setAfterPosition(Position *p, int num) {
	afterPosition = p;

	numAfterPos = num;
}

void Block::setBeforePosition(Position *p, int num) {
	beforePosition = p;

	numBeforePos = num;
}
