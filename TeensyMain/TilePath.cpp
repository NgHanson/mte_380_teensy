#include "TilePath.h"
#include "Constants.h"

TilePath::TilePath() {
}

TilePath::TilePath(Coordinate assignPath[], int assignPathCost, float assignDistCost, int assignPathIndex) {
  pathCost = assignPathCost;
  distCost = assignDistCost;
  for (int i = 0; i < MAX_PATH_FINDING_SIZE; i++) {
    path[i] = assignPath[i];
  }
  pathIndex = assignPathIndex;
}
