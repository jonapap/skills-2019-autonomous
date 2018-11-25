#include "Square.h"
#include "Functions.h"

Position Square::getApproachPosition(){
	int offsetHeading = mod(heading+180, 360);

	return {cos(toRadians(offsetHeading))*approachOffset+position.x, sin(toRadians(offsetHeading))*approachOffset+position.y};
}

double Square::getApproachHeading(){
	return heading;
}
