#include "PathFinding.h"
#include "TilePath.h"
#include "Coordinate.h"
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

const char validDir[] = {'u', 'd', 'r', 'l'};

Vector<Coordinate> deepCopy(Vector<Coordinate> vector) {
  Coordinate storage[20];
  Vector<Coordinate> copy(storage);

  for(int i = 0; i < vector.size(); i++) {
    copy.push_back(vector[i]);
  }

  return copy;
}

bool validLocation(int x, int y) {
    return (x < 6 && x >= 0 &&
          y < 6 && y >= 0 &&
          levelMap[y][x] == 'u'); // TODO: CONFIRM THIS, ONLY GO ON FLAT TILES
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

void printPath(Vector<Coordinate> list) {

  for(int i = 0; i < list.size(); i++) {
    Serial.print(list[i].x);
  }
  
}

void shortestPath(int objectiveX, int objectiveY) {
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
  
  Serial.print("CURR POS - X: ");
  Serial.print(startLocation.x);
  Serial.print(" Y: ");
  Serial.print(startLocation.y);
  Serial.print(" DIR: ");
  Serial.print(startLocation.dir);
  Serial.println();
  Serial.println();

  Serial.print("OBJ POS - X: ");
  Serial.print(objectiveX);
  Serial.print(" Y: ");
  Serial.print(objectiveY);
  Serial.println();
  Serial.println();
  
  Coordinate storage_array[20];
  Vector<Coordinate> startingPath(storage_array);
  startingPath.push_back(startLocation);
  Serial.print("STARTING PATH: ");
  Serial.println(startingPath.size());
  TilePath startingTilePath(startingPath, 0, euclideanDist(startLocation.x, objectiveX, startLocation.y, objectiveY));
  pq.push(startingTilePath);
  minCost[startLocation.y][startLocation.x][dirValue(startLocation.dir)] = 0;
  // --------------------//

  Serial.println("ENTERING WHILE LOOP");
  Serial.println();
  
  while(!pq.isEmpty()){
    TilePath currTilePath = pq.pop();
    int currPathCost = currTilePath.pathCost;

    Vector<Coordinate> currList = currTilePath.path;
    
    Coordinate curr = currList[currList.size() - 1];
    char currDir = curr.dir;

    Serial.print("CURR POS - X: ");
    Serial.print(curr.x);
    Serial.print(" Y: ");
    Serial.print(curr.y);
    Serial.print(" DIR: ");
    Serial.print(currDir);
    Serial.print(" CURR COST: ");
    Serial.print(currPathCost);
    Serial.print(" PQ Size: ");
    Serial.print(pq.count());
    Serial.println();

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

    Serial.print("CHECK FOR VALID FORWARD ADDED:  ");
    Serial.println(validLocation(forwardX, forwardY));
    Serial.print("PQ SIZE: ");
    Serial.println(pq.count());
    if (validLocation(forwardX, forwardY)) {
        if (forwardY == objectiveY && forwardX == objectiveX) { //WE REACHED THE OBJECTIVE
          printPath(currList);
          return;
        } else {
          int updatedPathCost = currPathCost + 1;          
          if (updatedPathCost < minCost[forwardY][forwardX][dirValue(currDir)] ) {
            double distCost = euclideanDist(forwardX, objectiveX, forwardY, objectiveY); //Heuristic for PriorityQueue comparator
            minCost[forwardY][forwardX][dirValue(currDir)] = updatedPathCost;
            Coordinate newCoord(forwardX, forwardY, currDir);
            Vector<Coordinate> newPath = deepCopy(currList);
            newPath.push_back(newCoord);
            TilePath newTilePath(newPath, updatedPathCost, distCost);
            pq.push(newTilePath);
          }
        }
      }
      int count = 0;
      // WE CAN ROTATE TO DIRECTIONS WHICH THE ROBOT CURRENTLY ISNT FACING
      for (int i = 0; i < 4; i++) {
        char c = validDir[i];
        if (c != currDir) {
          int updatedPathCost = currPathCost + 1;
          if (updatedPathCost < minCost[curr.y][curr.x][dirValue(c)] ) {
            Serial.print("DIR VALID: ");
            Serial.println(c);
            count++;
            double distCost = euclideanDist(curr.x, objectiveX, curr.y, objectiveY); //Heuristic for PriorityQueue comparator
            minCost[curr.y][curr.x][dirValue(c)] = updatedPathCost;
            Coordinate newCoord(curr.x, curr.y, c);
            Vector<Coordinate> newPath = deepCopy(currList);
            newPath.push_back(newCoord);
            TilePath newTilePath(newPath, updatedPathCost, distCost);
            pq.push(newTilePath);
          }
        }
      }
      Serial.print("CHECK FOR VALID ROTATION ADDED: ");
      Serial.println(count);
      Serial.print("PQ SIZE: ");
      Serial.println(pq.count());
      Serial.println();
  }
  // NO PATH FOUND
  Serial.println("NO PATH FOUND");
  // FIGURE OUT RETURN
}
