#include "Block.h"
#include "Functions.h"
#include "DEBUG.h"

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
	robot.goToPingDistance(100, 2);
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

void Block::setAfterPosition(Position p){
	afterPosition = p;
	hasAfterPos = true;
}

void Block::setBeforePosition(Position p){
	beforePosition = p;
	hasBeforePos = true;
}
