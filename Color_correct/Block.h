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
	Block(Position p, int h, int s) : position(p), heading(h), approachSide(s) {}

	Position position;
	int heading;
	int color = 0;
	int approachSide;
	Position path[2];


	Position getLastPoint();

private:
	const double shiftLength = 14.866;
	const int shiftAngle = 47.73;

};

#endif /* BLOCK_H_ */
