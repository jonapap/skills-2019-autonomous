OmniRobot robot(47.5, 8, 90);

Square redSquare( { 18.5, 75.8 }, 135, Square::red);
Square blueSquare( { 75.8, 75.8 }, 45, Square::blue);
Square yellowSquare( { 48, 82.25 }, 90, Square::yellow);

Block blocks[] = { Block( { 44.5, 40.5}, 75, RIGHTAPPROACH, blueSquare), //block 1
Block( { 52.5, 55}, 255, LEFTAPPROACH, blueSquare), //block 2
Block( { 58, 47.5 }, 180, RIGHTAPPROACH, yellowSquare),  //block 3
Block( { 43.5, 55}, 285, RIGHTAPPROACH, redSquare), //block 4
Block( { 38.5, 47.5}, 0, LEFTAPPROACH, yellowSquare), //block 5
Block( { 52, 40 }, 105, RIGHTAPPROACH, redSquare) //block 6
		};

void afterYellowSquare(){
	robot.turn(45, 100);
}

void setupCourt() {
	blocks[0].setAfterPosition({77.5, 17}); //block 1 - position intermédiaire après avoir pris le bloc

	blocks[2].setAfterPosition({69, 66.5});

	blocks[4].setAfterPosition({26, 68});

	blocks[5].setBeforePosition(new Position[2]{{24, 55}, {25, 25}}, 2);
	blocks[5].setAfterPosition({19, 27});

	yellowSquare.afterFunction = afterYellowSquare;
}
