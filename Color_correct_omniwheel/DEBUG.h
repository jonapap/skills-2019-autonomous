#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG //comment this to disable debugging

#ifdef DEBUG
 #define DEBUG_PRINTLN(x)  Serial.println (x) //if DEBUG is set, define DEBUG_PRINTLN to be Serial.println
 #define DEBUG_PRINT(x)  Serial.print (x) //similar to above
#else
 #define DEBUG_PRINT(x) //if DEBUG is not set, DEBUG_PRINT and DEBUG_PRINTLN will do nothing
 #define DEBUG_PRINTLN(x)
#endif

#endif
