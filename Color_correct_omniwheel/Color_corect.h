#ifndef _Color_corect_H_
#define _Color_corect_H_

#include "Arduino.h"
#include "TETRIX_PRIZM/PRIZM.h"
#include "DEBUG.h"
#include "Block.h"
#include "Square.h"
#include "Functions.h"
#include "OmniRobot.h"


void grabBlock();
void printXY();
void testLocation();
void cycleBlocks();
void serialBlocks();
void depositBlock(Square &s);
void goToBlock(Block &b);
void goToSquare(Block &b);
void alignWithLine(int side);
void alignWithSquare(Square &s);


#endif /* _Color_corect_H_ */
