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

inline boolean isZero(int a){
	return a == 0 ? true : false;
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

inline int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

#endif
