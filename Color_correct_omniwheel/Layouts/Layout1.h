OmniRobot robot(14.5, 14.5, 45);

Square redSquare( { 17.5, 76.25 }, 135, Square::red);
Square blueSquare( { 75.5, 76.5 }, 45, Square::blue);
Square yellowSquare( { 76.5, 19.5 }, 315, Square::yellow, 50);

Block blocks[] = { Block( { 42, 47 }, 0, RIGHTAPPROACH, blueSquare), //block 1
Block( { 90.5, 46.75}, 0, LEFTAPPROACH, blueSquare), //block 2
Block( { 48.5, 90 }, 90, LEFTAPPROACH, redSquare),  //block 3
Block( { 50, 52}, 240, LEFTAPPROACH, yellowSquare), //block 4
Block( { 50.5, 43 }, 120, RIGHTAPPROACH, redSquare), //block 5
Block( { 47.5, 4.5 }, 270, RIGHTAPPROACH, yellowSquare) //block 6
		};

void beforeRedSquare(Square &s){
	if(s.numOfBlocks == 1) {
		robot.advanceRelative(5, 100, 90);
	}
}

void setupCourt() {
	blocks[0].setAfterPosition({30, 70}); //block 1 - position intermédiaire après avoir pris le bloc

	blocks[3].setAfterPosition({75, 51}); //block 4

	blocks[4].setAfterPosition(new Position[2]{{70, 48}, {60, 70}}, 2); //block 5

	blocks[5].setBeforePosition({ 30, 25 }); //block 6 - position intermédiaire avant d'avoir pris le bloc

	//redSquare.setBeforeFunction(beforeRedSquare);
}
