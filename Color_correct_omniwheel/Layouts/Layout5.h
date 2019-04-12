OmniRobot robot(47, 8, 90);

Square redSquare( { 31.5, 47 }, 45, Square::red);
Square blueSquare( { 64, 46.5 }, 135, Square::blue);
Square yellowSquare( { 46.5, 73 }, 270, Square::yellow);

Block blocks[] = { Block( { 4.25, 47.5 }, 180, LEFTAPPROACH, redSquare), //block 1
Block( { 8.75, 86 }, 135, LEFTAPPROACH, yellowSquare),  //block 2
Block( { 86, 86 }, 45, LEFTAPPROACH, yellowSquare), //block 3
Block( { 90.25, 47.5 }, 0, LEFTAPPROACH, redSquare), //block 4
Block( { 8.5, 8.5 }, 225, RIGHTAPPROACH, blueSquare), //block 5
Block( { 85.75, 8.5 }, 315, LEFTAPPROACH, blueSquare) //block 6
		};

void afterYellowSquare(Square &s){
	robot.turn(45, 100);
	robot.advanceRelative(s.numOfBlocks == 1 ? -10 : 10, 100, 45);
}

void setupCourt() {
	//yellowSquare.setAfterFunction(afterYellowSquare);

	blocks[3].setBeforePosition({67,77});
	blocks[3].setAfterPosition({47, 22});
}
