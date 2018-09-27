// Do not remove the include below
#include "Color_corect.h"

PRIZM prizm;
Robot robot;

//The setup function is called once at startup of the sketch
void setup() {

	Serial.begin(9600);
	prizm.PrizmBegin();
	robot.invertMotor(1, 1);


	//prizm.setMotorSpeeds(300, 300);
	//robot.rampSpeed(720, 1000);
	//robot.advanceCM(30, 300);
	//prizm.setMotorPowers(100,100);
}

// The loop function is called in an endless loop
void loop() {
 while(Serial.available()==0);
 double data = Serial.parseFloat();
 Serial.println(data);
 robot.turnvalue = data;
 robot.turn(90, 100);
}
