// Do not remove the include below
#include "Color_corect.h"


PRIZM prizm;
Robot robot;

//The setup function is called once at startup of the sketch
void setup() {

#ifdef DEBUG
	Serial.begin(9600);
#endif

	prizm.PrizmBegin();
	robot.invertMotor(2, 1);

	//robot.advanceUntilLine(100, Robot::yellow, 5);
	//prizm.setMotorSpeeds(300, 300);
	//robot.rampSpeed(720, 1000);
	//robot.advanceCM(30, 300);
	//prizm.setMotorPowers(100,100);
}


void loop() {/*
	Serial.println("X :");
	while(Serial.available() == 0);
	int x = Serial.parseInt();
	Serial.println("Y :");
	while(Serial.available() == 0);
	int y = Serial.parseInt();
	robot.goToLocation(x, y, 100);
	robot.setHeading(90, 100);*/

	while(Serial.available() == 0);
	double x = Serial.parseFloat();
	Serial.println(x);

	robot.turnvalue = x;
	robot.turn(360, 100);

}
