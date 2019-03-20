#include "PathFinding.h"
#include "PriorityQueue.h"
#include "Globals.h"
#include <Vector.h>
// #include <StandardCplusplus.h>
// #include <serstream>
// #include <string>
// #include <vector>
// #include <iterator>
const int ELEMENT_COUNT_MAX = 5;

int const MAX_VALUE = 999;

class Coordinate {
	public:
		int x;
		int y;
		char dir;
		Coordinate(int x, int y, char dir) {
			x = x;
			y =y;
			dir = dir;
		}
};

// class TilePath {
// public:
// 	int pathCost;
// 	double distCost;

// }


void shortestPath(int objectiveX, int objectiveY) {
int storage_array[ELEMENT_COUNT_MAX];
Vector<int> vector(storage_array);
vector.push_back(77);
  int minCost[6][6][4];
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 4; k++) {
        minCost[i][j][k] = MAX_VALUE;
      }
    }
  }

}