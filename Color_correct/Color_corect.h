#ifndef _Color_corect_H_
#define _Color_corect_H_

#include "Arduino.h"
#include "Robot.h"
#include "TETRIX_PRIZM/PRIZM.h"
#include "DEBUG.h"
#include "Block.h"


void grabBlock(int side);
void printXY();
void testLocation();
void cycleBlocks();
void serialBlocks();
void goToSquare(int color);


#endif /* _Color_corect_H_ */
