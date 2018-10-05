// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Color_corect_H_
#define _Color_corect_H_

#include "Arduino.h"
#include "Robot.h"
#include "TETRIX_PRIZM/PRIZM.h"
#include "DEBUG.h"
#include "Block.h"
//add your includes for the project Color_corect here


//end of add your includes here


//add your function definitions for the project Color_corect here

void grabBlock(int side);
void printXY();
void testLocation();
void cycleBlocks();
void goToSquare(int color);


//Do not add code below this line
#endif /* _Color_corect_H_ */
