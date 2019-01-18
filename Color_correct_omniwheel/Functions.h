#ifndef FUNCTION_H
#define FUNCTION_H

#include <Arduino.h>
#include "DEBUG.h"


inline double toRadians(double x) {
	return x * (PI / 180);
}

inline double toDegrees(double x) {
	return x / (PI / 180);
}

template<typename T>
inline T mod(T x, int y) {
	return x - y * floor(x / y);
}

template<typename T, typename U>
inline boolean inRange(T x, T y, U r) {
	return abs(x-y) <= r;
}

template<typename T>
void sendln(T v) {
	DEBUG_PRINT(v);
	DEBUG_PRINTLN("");
}

template<typename T, typename ... Args>
void sendln(T first, Args ... args) {
	DEBUG_PRINT(first);
	sendln(args...);
}

#endif
