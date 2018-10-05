#include "Block.h"

#define toRadians(x) x*(PI/180)
#define toDegrees(x) x/(PI/180)
#define mod(x,y) (x)-y*floor((x)/y)

Position Block::getLastPoint(){
	int angle = mod(mod(heading+180,360) + (shiftAngle*approachSide),360);

	double deltaX = cos(toRadians(angle))*shiftLength;
	double deltaY = sin(toRadians(angle))*shiftLength;

	double x = position.x + deltaX;
	double y = position.y + deltaY;

	return {x,y};
}

double Block::getApproachHeading(){
	return mod(heading + (90*approachSide), 360);
}
