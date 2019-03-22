OmniRobot robot(14.2, 14.5, 45);

Square redSquare( { 18.5, 75.8 }, 135, Square::red);
Square blueSquare( { 75.8, 75.8 }, 45, Square::blue);
Square yellowSquare( { 75.8, 18.5 }, 315, Square::yellow);

Block blocks[] = { Block( { 41.75, 35.5 }, 0, RIGHTAPPROACH, redSquare), //block 1
Block( { 41.75, 47.5}, 0, LEFTAPPROACH, blueSquare), //block 2
Block( { 52.5, 59.5 }, 180, RIGHTAPPROACH, blueSquare),  //block 3
Block( { 52.5, 47.5}, 180, RIGHTAPPROACH, redSquare), //block 4
Block( { 41.75, 59.5}, 0, LEFTAPPROACH, yellowSquare), //block 5
Block( { 52.5, 35.5 }, 180, LEFTAPPROACH, yellowSquare) //block 6
		};

void setupCourt() {
	blocks[1].setAfterPosition({30, 83}); //block 2 - position intermédiaire après avoir pris le bloc

	blocks[3].setAfterPosition({60, 83});

	blocks[4].setAfterPosition({34, 16});

}
