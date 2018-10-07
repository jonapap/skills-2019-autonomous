#include "Square.h"

#define toRadians(x) x*(PI/180)
#define toDegrees(x) x/(PI/180)
#define IN_RANGE(x, y, r) abs(x-y) <= r
#define mod(x,y) (x)-y*floor((x)/y)

Position Square::getApproachPosition(){
	int offsetHeading = mod(heading+180, 360);

	return {cos(toRadians(offsetHeading))*approachOffset+position.x, sin(toRadians(offsetHeading))*approachOffset+position.y};
}

double Square::getApproachHeading(){
	return heading;
}
