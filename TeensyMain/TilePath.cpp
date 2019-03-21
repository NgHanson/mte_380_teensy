#include "TilePath.h"
#include <Vector.h>

TilePath::TilePath() {
}

TilePath::TilePath(Coordinate assignPath[], int assignPathCost, float assignDistCost, int assignPathIndex) {
  pathCost = assignPathCost;
  distCost = assignDistCost;
  for (int i = 0; i < 60; i++) {
    path[i] = assignPath[i];
  }
  pathIndex = assignPathIndex;
}
