#ifndef Coordinate_h
#define Coordinate_h

#include <Arduino.h>

class Coordinate {
  public:
    int x;
    int y;
    char dir;
    Coordinate();
    Coordinate(int assignX, int assignY, char assignDir);
};

#endif
