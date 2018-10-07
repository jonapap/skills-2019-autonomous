#ifndef BLOCK_H_
#define BLOCK_H_

#include <Arduino.h>
#include "Robot.h"
#include "DEBUG.h"
#include "Square.h"

#define REDSQUARE 0
#define BLUESQUARE 1
#define YELLOWSQUARE 2

#define LEFTAPPROACH -1
#define RIGHTAPPROACH 1

class Block {
public:
	Block(Position p, int h, int s, Square &c) :
			position(p), heading(h), approachSide(s), square(c) {
	}
	Block(Position p, int h, int s, Square &c, Position pa[]) :
			position(p), heading(h), approachSide(s), square(c) {
		path[0] = pa[0];
		path[1] = pa[1];
	}

	Position position;
	double heading;
	int approachSide;
	Square &square;
	Position path[2];

	Position getLastPoint();
	double getApproachHeading();
	Position getRobotLinePosition();
	Square &getSquare();

private:
	const double shiftLength = 13.45;
	const double shiftAngle = 48.01;

};

#endif /* BLOCK_H_ */
