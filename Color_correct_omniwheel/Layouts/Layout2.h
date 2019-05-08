OmniRobot robot(14.5, 14.5, 45);

Square redSquare( { 18.5, 76 }, 135, Square::red);
Square blueSquare( { 76, 76 }, 45, Square::blue);
Square yellowSquare( { 76.5, 19 }, 315, Square::yellow);

Block blocks[] = { Block( { 41.75, 35 }, 0, RIGHTAPPROACH, redSquare), //block 1
Block( { 41.75, 47}, 0, LEFTAPPROACH, blueSquare), //block 2
Block( { 52.75, 59 }, 180, RIGHTAPPROACH, blueSquare),  //block 3
Block( { 52.75, 47}, 180, RIGHTAPPROACH, redSquare), //block 4
Block( { 41.75, 59}, 0, LEFTAPPROACH, yellowSquare), //block 5
Block( { 52.75, 35 }, 180, LEFTAPPROACH, yellowSquare) //block 6
		};

void setupCourt() {
	blocks[1].setAfterPosition({30, 83}); //block 2 - position intermédiaire après avoir pris le bloc

	blocks[3].setAfterPosition({60, 83});

	blocks[4].setAfterPosition({34, 16});

}
