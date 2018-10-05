#ifndef BLOCK_H_
#define BLOCK_H_

#include <Arduino.h>
#include "Robot.h"
#include "DEBUG.h"

#define REDSQUARE 0
#define BLUESQUARE 1
#define YELLOWSQUARE 2

#define LEFTAPPROACH -1
#define RIGHTAPPROACH 1

class Block {
public:
	Block(Position p, int h, int s, int c) : position(p), heading(h), approachSide(s), color(c) {}
	Block(Position p, int h, int s, int c, Position pa[]) : position(p), heading(h), approachSide(s), color(c) {
		path[0] = pa[0];
		path[1] = pa[1];
	}

	Position position;
	double heading;
	int approachSide;
	int color;
	Position path[2];


	Position getLastPoint();
	double getApproachHeading();

private:
	const double shiftLength = 14.866;
	const int shiftAngle = 47.73;

};

#endif /* BLOCK_H_ */
