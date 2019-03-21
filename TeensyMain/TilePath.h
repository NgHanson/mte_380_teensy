#ifndef TilePath_h
#define TilePath_h

#include <Arduino.h>
#include "Constants.h"
#include "Coordinate.h"

class TilePath {
  public:
    int pathCost;
    float distCost;
    Coordinate path[MAX_PATH_FINDING_SIZE];
    int pathIndex;

    TilePath();
    TilePath(Coordinate assignPath[], int assignPathCost, float assignDistCost, int assignPathIndex);
};

#endif
