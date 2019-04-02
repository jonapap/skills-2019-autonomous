#ifndef SQUARE_H_
#define SQUARE_H_

#include "Types.h"
#include "OmniRobot.h"

class OmniRobot;

class Square {
public:
	Square(Position p, double h, RGB c, int cE = 10) :
			position(p), heading(h), color(c), colorError(cE){
	}

	Position getApproachPosition();
	double getApproachHeading();
	Position getRobotAlignedPosition();
	RGB getColor();
	int getColorError();
	void setAfterFunction(void(*)(Square&));
	void callAfterFunction();

	static const RGB white;
	static const RGB red;
	static const RGB blue;
	static const RGB yellow;

	int numOfBlocks = 0;



private:
	Position position;
	double heading;
	RGB color;

	int colorError;

	void (*afterFunction)(Square&) = NULL;

	static constexpr int approachOffset = 12;

	static constexpr double alignedOffsetLength = 5;
	static constexpr double alignedOffsetAngle = 0;
};

#endif /* SQUARE_H_ */
