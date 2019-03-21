#include "PathFinding.h"
#include "TilePath.h"
#include "Coordinate.h"
#include "PriorityQueue.h"
#include "Globals.h"

const int ELEMENT_COUNT_MAX = 5;

int const MAX_VALUE = 999;

const char validDir[] = {'u', 'd', 'r', 'l'};

void deepCopy(Coordinate toBeCopied[], Coordinate newCopy[], int valsToCopy) {
  for (int i = 0; i <= valsToCopy; i++) {
    Coordinate newCopyCoord = toBeCopied[i];
    Coordinate copyVal(newCopyCoord.x, newCopyCoord.y, newCopyCoord.dir);
    newCopy[i] = copyVal;
  }
}

bool validLocation(int x, int y) {
    return (x < 6 && x >= 0 && y < 6 && y >= 0 &&
          (levelMap[y][x] == 'u' || levelMap[y][x] == 'r'));
}

float euclideanDist(int x1, int x2, int y1, int y2) {
  return pow( pow(x1 - x2,2) + pow(y1-y2,2),0.5);
}

char convertHeadingToCharDirection(float heading) {
  //PROBABLY SHOULDNT ROUND TO NEARIST 90
  if ((345.0 <= heading && heading < 360.0) || (heading >= 0.0 && heading <= 15.0)) {
    return 'u';
  } else if (heading >= 75.0 && heading <= 105.0) {
    return 'r';
  } else if (heading >= 165.0 && heading <= 195.0) {
    return 'd';
  } else if (heading >= 255.0 && heading <= 285.0) {
    return 'l';
  } else { // CURRENTLY HEADING IS COMPLETELY OFF FROM a 90 deg
    Serial.print("WRONG");
    return '-'; //SHOULD PROB THROW ERR OR PERFORM RE ALIGN 
  }
}

int dirValue(char c) {
    if (c == 'u') {
      return 0;
    } else if(c == 'd') {
      return 1;
    } else if (c == 'r') {
      return 2;
    } else if (c == 'l') {
      return 3;
    } else {
      return -1;
    }
}

// Sort by tile with the closts euclidean distance
bool comparePaths(TilePath path1, TilePath path2){
  return path1.distCost < path2.distCost;
}

void printPath(Coordinate list[], int listSize, Coordinate currTile) {
  Serial.println("FINAL PATH");
  for(int i = 0; i < listSize; i++) {
    Serial.print("X: ");
    Serial.print(list[i].x);
    Serial.print(" Y: ");
    Serial.print(list[i].y);
    Serial.print(" DIR: ");
    Serial.println(list[i].dir);
  }
  Serial.print("X: ");
  Serial.print(currTile.x);
  Serial.print(" Y: ");
  Serial.print(currTile.y);
  Serial.print(" DIR: ");
  Serial.println(currTile.dir);
}

void printTilePath(TilePath currTilePath) {
  int currPathIndex = currTilePath.pathIndex;
  int currPathCost = currTilePath.pathCost;

  Coordinate* currList = currTilePath.path;
  
  Coordinate curr = currList[currPathIndex];
  char currDir = curr.dir;
  Serial.print("CURR TILE INDEX: ");
  Serial.print(currPathIndex);
  Serial.print(" CURR POS - X: ");
  Serial.print(curr.x);
  Serial.print(" Y: ");
  Serial.print(curr.y);
  Serial.print(" DIR: ");
  Serial.print(currDir);
  Serial.print(" CURR COST: ");
  Serial.println(currPathCost);
  Serial.print(" DIST COST: ");
  Serial.println(currTilePath.distCost);
  Serial.println();
}

void printCoord(Coordinate curr) {
  Serial.print(" CURR POS - X: ");
  Serial.print(curr.x);
  Serial.print(" Y: ");
  Serial.print(curr.y);
  Serial.print(" DIR: ");
  Serial.print(curr.dir);
  Serial.println();
}

void copyIntoResults(Coordinate coordResult[], Coordinate shortestPath[], int pathSize) {
  for (int i = 0; i <= pathSize; i++) {
    coordResult[i] = shortestPath[i];
  }
  Coordinate exitCoord(-1, -1, '-');
  coordResult[pathSize+1] = exitCoord;
}

void shortestPath(Coordinate coordResult[], int objectiveX, int objectiveY) {

  // --- SETUP STEPS --- //
  int minCost[6][6][4];
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6; j++) {
      for (int k = 0; k < 4; k++) {
        minCost[i][j][k] = MAX_VALUE;
      }
    }
  }

  PriorityQueue<TilePath> pq = PriorityQueue<TilePath>(comparePaths);

  // --- ADD START POS --- //
  char currHeading = convertHeadingToCharDirection(cwHeading);
  Coordinate startLocation(xPos, yPos, currHeading); // OUR GLOBAL CURRENT LOCATION
  
  Coordinate startingPath[60];
  startingPath[0] = startLocation;

  TilePath startingTilePath(startingPath, 0, euclideanDist(startLocation.x, objectiveX, startLocation.y, objectiveY), 0);
  pq.push(startingTilePath);
  minCost[startLocation.y][startLocation.x][dirValue(startLocation.dir)] = 0;
  // --------------------//
  
  while(!pq.isEmpty()){
    TilePath currTilePath = pq.pop();
    int currPathIndex = currTilePath.pathIndex;
    int currPathCost = currTilePath.pathCost;

    Coordinate* currList = currTilePath.path;
    
    Coordinate curr = currList[currPathIndex];
    char currDir = curr.dir;

    //WE CAN ONLY MOVE FORWARD IN THE DIRECTION WE'RE FACING
    int forwardX = curr.x;
    int forwardY = curr.y;

    // Determine the next grid location if we move forward given the current direction
    if (currDir == 'u') {
      forwardY++;
    } else if (currDir == 'l') {
      forwardX++;
    } else if (currDir == 'r') {
      forwardX--;
    } else if (currDir == 'd') {
      forwardY--;
    }

    if (validLocation(forwardX, forwardY)) {
      if (forwardY == objectiveY && forwardX == objectiveX) { //WE REACHED THE OBJECTIVE
        //printPath(currList, currPathIndex, curr);
        currList[currPathIndex+1] = curr;
        copyIntoResults(coordResult, currList, currPathIndex+1);
        return;
      } else {
        int updatedPathCost = currPathCost + 1;          
        if (updatedPathCost < minCost[forwardY][forwardX][dirValue(currDir)] ) {
          double distCost = euclideanDist(forwardX, objectiveX, forwardY, objectiveY); //Heuristic for PriorityQueue comparator
          minCost[forwardY][forwardX][dirValue(currDir)] = updatedPathCost;
          Coordinate newCoord(forwardX, forwardY, currDir);
          Coordinate newPath[60];
          deepCopy(currList, newPath, currPathIndex);
          newPath[currPathIndex+1] = newCoord;
          TilePath newTilePath(newPath, updatedPathCost, distCost, currPathIndex+1);
          pq.push(newTilePath);
        }
      }
    }

    // WE CAN ROTATE TO DIRECTIONS WHICH THE ROBOT CURRENTLY ISNT FACING
    for (int i = 0; i < 4; i++) {
      char c = validDir[i];
      if (c != currDir) {
        int updatedPathCost = currPathCost + 1;
        if (updatedPathCost < minCost[curr.y][curr.x][dirValue(c)] ) {
          double distCost = euclideanDist(curr.x, objectiveX, curr.y, objectiveY); //Heuristic for PriorityQueue comparator
          minCost[curr.y][curr.x][dirValue(c)] = updatedPathCost;
          Coordinate newCoord(curr.x, curr.y, c);
          Coordinate newPath[60];
          deepCopy(currList, newPath, currPathIndex);
          newPath[currPathIndex+1] = newCoord;
          TilePath newTilePath(newPath, updatedPathCost, distCost, currPathIndex+1);
          pq.push(newTilePath);
        }
      }
    }

  }
  // NO PATH FOUND
  Serial.println("NO PATH FOUND");
  Coordinate exitCoord(-1, -1, '-');
  coordResult[0] = exitCoord;
}

void getPath(int pathResult[], Coordinate objectiveTile) {

  Coordinate coordResult[60]; //MAX SIZE FOR NOW

  shortestPath(coordResult, objectiveTile.x, objectiveTile.y);

  int pathSize = 0;
  for (int i = 0; i < 60; i++) {
    if (coordResult[i].x == -1) { // SUPER HACKY
      break;
    }
    printCoord(coordResult[i]);
    pathSize++;
  }
  Serial.print("PATH SIZE: ");
  Serial.println(pathSize);

  /*
  0 - forward
  1 - rotate 0
  2 - rotate 90
  3 - rotate 180
  4 - rotate 270
  -1 - end
  */

  // TODO: ADJUST BASED ON OUR FINAL COORDINATE SYSTEM
  for (int i = 0; i < pathSize; i++) {
    char start = coordResult[i].dir;
    char next = coordResult[i+1].dir;

    if (start == next) { //MAINTAIN SAME DIR ... MEANING IT MOVED FORWARD
      pathResult[i] = 0;
    } else if (next == 'u') {
      pathResult[i] = 1;
    } else if (next == 'd') {
      pathResult[i] = 3;
    } else if (next == 'l') {
      pathResult[i] = 4;
    } else if (next == 'r') {
      pathResult[i] = 2;
    }


  }
  pathResult[pathSize] = -1;
}
