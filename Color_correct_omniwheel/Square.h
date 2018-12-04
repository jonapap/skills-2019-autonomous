#ifndef SQUARE_H_
#define SQUARE_H_

#include "OmniRobot.h"

class Square {
public:
	Square(Position p, double h, RGB c, int cE = 10) :
			position(p), heading(h), color(c), colorError(cE){
	}
	Position getApproachPosition();
	double getApproachHeading();
	Position getRobotAlignedPosition();
	void align(OmniRobot &robot);

	static const RGB white;
	static const RGB red;
	static const RGB blue;
	static const RGB yellow;

private:
	Position position;
	double heading;
	RGB color;

	int colorError;

	static constexpr int approachOffset = 12;

	static constexpr double alignedOffsetLength = 6.5;
	static constexpr double alignedOffsetAngle = 0;
};

#endif /* SQUARE_H_ */
