#include "PathFinding.h"
#include "PriorityQueue.h"
#include "Globals.h"

int const MAX_VALUE = 999;

void shortestPath(int objectiveX, int objectiveY) {

  int minCost[6][6][4];
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 4; k++) {
        minCost[i][j][k] = MAX_VALUE;
      }
    }
  }

}