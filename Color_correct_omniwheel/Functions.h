#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>

inline double toRadians(double x){
	return x*(PI/180);
}

inline double toDegrees(double x){
	return x/(PI/180);
}

template <typename T>
inline T mod(T x, int y){
	return x-y*floor(x/y);
}

template <typename T, typename U>
inline boolean inRange(T x, T y, U r){
	return abs(x-y) <= r;
}

#endif
