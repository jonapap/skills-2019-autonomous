OmniRobot robot(47.375, 47.375, 90);

Square redSquare( { 18.5, 75.8 }, 135, Square::red);
Square blueSquare( { 75.8, 75.8 }, 45, Square::blue);
Square yellowSquare( { 48, 82.25 }, 90, Square::yellow);

Block blocks[] = { Block( { 4.5, 47.375}, 180, RIGHTAPPROACH, yellowSquare), //block 1
Block( { 90.25, 47.375}, 0, LEFTAPPROACH, yellowSquare), //block 2
Block( { 17, 4.5 }, 270, LEFTAPPROACH, redSquare),  //block 3
Block( { 37, 4.5}, 270, RIGHTAPPROACH, blueSquare), //block 4
Block( { 57.75, 4.5}, 270, LEFTAPPROACH, blueSquare), //block 5
Block( { 77.75, 4.5 }, 270, RIGHTAPPROACH, redSquare) //block 6
		};

void afterYellowSquare(Square &s){
	robot.turn(45, 100);
	robot.advanceRelative(s.numOfBlocks == 1 ? -10 : 10, 100, 45);
}

void setupCourt() {
	yellowSquare.setAfterFunction(afterYellowSquare);
}
