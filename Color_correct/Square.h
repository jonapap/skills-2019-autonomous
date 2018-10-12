#ifndef SQUARE_H_
#define SQUARE_H_

#include "Robot.h"

class Square {
public:
	Square (Position p, double h) : position(p), heading(h) {}
	Position getApproachPosition();
	double getApproachHeading();

private:
	Position position;
	double heading;

	static constexpr int approachOffset = 12;
};



#endif /* SQUARE_H_ */
