#ifndef BLOCK_H_
#define BLOCK_H_

#include <Arduino.h>

#include "OmniRobot.h"
#include "Square.h"

#define LEFTAPPROACH -1
#define RIGHTAPPROACH 1

class Block {
public:
	Block(Position p, int h, int s, Square &c) :
			position(p), heading(h), approachSide(s), square(c) {
	}

	~Block();

	Position position; //block's position
	Position *beforePosition = NULL; //positions before going to block
	Position *afterPosition = NULL; //positions after going to block

	int numBeforePos = 0; //keep track of how many positions we have for each
	int numAfterPos = 0;

	double heading;
	int approachSide;
	Square &square;

	Position getLastPoint();
	double getApproachHeading();
	Position getRobotAlignedPosition();
	Square &getSquare();
	void align(OmniRobot &robot);

	void setBeforePosition(Position *p, int num);
	void setAfterPosition(Position *p, int num);
	void setAfterPosition(Position p);
	void setBeforePosition(Position p);

	void setAfterFunction(void (*)(Block&));
	void callAfterFunction();
	void setBeforeFunction(void (*)(Block&));
	void callBeforeFunction();

private:
	void (*afterFunction)(Block&) = NULL;
	void (*beforeFunction)(Block&) = NULL;

	static constexpr double shiftLength = 15; //length and angle from middle of block to where the robot should go before aligning with the block
	static constexpr double shiftAngle = 20;

	static constexpr double blockGrabOffsetLength = 10; //length and angle to robot's center when aligned with block
	static constexpr double blockGrabOffsetAngle = 0;

};

#endif /* BLOCK_H_ */
