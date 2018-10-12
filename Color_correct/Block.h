#ifndef BLOCK_H_
#define BLOCK_H_

#include <Arduino.h>
#include "Robot.h"
#include "Square.h"

#define LEFTAPPROACH -1
#define RIGHTAPPROACH 1

class Block {
public:
	Block(Position p, int h, int s, Square &c) :
			position(p), heading(h), approachSide(s), square(c) {
	}


	Position position;
	double heading;
	int approachSide;
	Square &square;

	Position getLastPoint();
	double getApproachHeading();
	Position getRobotLinePosition();
	Square &getSquare();

private:
	static constexpr double shiftLength = 14.21;
	static constexpr double shiftAngle = 50.71;
};

#endif /* BLOCK_H_ */
