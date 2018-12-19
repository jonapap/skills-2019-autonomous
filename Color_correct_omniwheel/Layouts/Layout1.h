OmniRobot robot(14.2, 14.5, 45);

Square redSquare( { 18.5, 75.8 }, 135, Square::red);
Square blueSquare( { 75.8, 75.8 }, 45, Square::blue);
Square yellowSquare( { 75.8, 18.5 }, 315, Square::yellow, 50);

Block blocks[] = { Block( { 41.8, 46.4 }, 0, RIGHTAPPROACH, blueSquare), //block 1
Block( { 89.9, 47.2}, 0, LEFTAPPROACH, blueSquare), //block 2
Block( { 46, 90.1 }, 90, LEFTAPPROACH, redSquare),  //block 3
Block( { 50.0, 51}, 240, RIGHTAPPROACH, yellowSquare), //block 4
Block( { 50, 41.7 }, 120, RIGHTAPPROACH, redSquare), //block 5
Block( { 46, 4.6 }, 270, RIGHTAPPROACH, yellowSquare) //block 6
		};

void setupCourt() {
	blocks[0].setAfterPosition({30, 70}); //block 1

	blocks[3].setAfterPosition({75, 51}); //block 4

	blocks[4].setAfterPosition({ 26, 15}); //block 5

	blocks[5].setBeforePosition({ 75, 48}); //block 6

}
