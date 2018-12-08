OmniRobot robot(47.5, 7.5, 90);

Square redSquare( { 28, 82.5 }, 90, Square::red);
Square blueSquare( { 68, 82.5 }, 90, Square::blue);
Square yellowSquare( { 49, 82.5 }, 90, Square::yellow, 50);

Block blocks[] = { Block( { 66.75, 5.5 }, 270, RIGHTAPPROACH, yellowSquare), //block 1
Block( { 80, 33 }, 0, LEFTAPPROACH, redSquare), //block 2
Block( { 4.25, 63.5 }, 180, LEFTAPPROACH, redSquare),  //block 6
Block( { 15.5, 33 }, 180, RIGHTAPPROACH, yellowSquare), //block 5
Block( { 29.5, 5.5 }, 270, LEFTAPPROACH, blueSquare), //block 4
Block( { 90.75, 63.5 }, 0, RIGHTAPPROACH, blueSquare) //block 3
		};
