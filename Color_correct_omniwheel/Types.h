#ifndef TYPES_H_
#define TYPES_H_

#include <Arduino.h>

struct RGB {
	int red, green, blue;

	boolean isColor(const RGB &color, int error);

};

struct Position {
	double x, y;
};

struct EncoderValues {
	long enc1, enc2, enc3, enc4;
};

struct Vector {
	double distance, angle;
};



#endif /* TYPES_H_ */
