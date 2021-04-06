#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>

using std::vector;
using std::cout;
using std::endl;

#define MAX_VALUE 999
#define MIN_VALUE -999.0
#define APPLE 1
#define ORANGE 2
#define PINEAPPLE 3
#define STRAWBERRY 4
#define WATERMELON 5
#define WALL_VALUE 0
#define WALL '9'
#define MAPSIZE_X 25
#define MAPSIZE_Y 25
#define SMALL_MAP_SIZE 5

typedef struct Position {
	int x;
	int y;
}Pos;

class PlayerAction {
public:
	int GetTwoCellDistance(Position cell1 ,Position cell2) {
		return abs(cell1.x - cell2.x) + abs(cell1.y - cell2.y);
	}

	//Compare the distance between fruit and players and enemies
	int GetFruitActualScore(Position fruitPos, int fruitScore, Position playerPos, vector<Position> enemyPos) {
		int distToPlayer = GetTwoCellDistance(fruitPos, playerPos);
		int distToEnemy;
		for(int i = 0; i < enemyPos.size(); i++) {
			distToEnemy += GetTwoCellDistance(fruitPos, enemyPos[i]);
		}

		int fruitActualScore = (1 - (distToPlayer / (distToEnemy + distToPlayer))) * fruitScore;

        return fruitActualScore;
	}


	vector<vector<float>> GetSmallMapScore(char map[MAPSIZE_X][MAPSIZE_Y], Position playerPos, vector<Position> enemyPos) {	
		vector<vector<float>> smallMapScore(SMALL_MAP_SIZE, vector<float>(SMALL_MAP_SIZE, 0.0));

		for(int i = 0; i < MAPSIZE_X; i++) {	
			for(int j = 0; j < MAPSIZE_Y; j++) {
				Position position;
				position.x = j;
				position.y = i;
                if(WALL == map[i][j]) {
                    smallMapScore[i/SMALL_MAP_SIZE][j/SMALL_MAP_SIZE] += WALL_VALUE;
                } else {
                    smallMapScore[i/SMALL_MAP_SIZE][j/SMALL_MAP_SIZE] += GetFruitActualScore(position, (int)(map[i][j] - '0'), playerPos, enemyPos);
                }
            }
		}

		return smallMapScore;
	}

	//return biggest small map position
	Position GetBiggestSmallMapPosition(vector<vector<float>> smallMapScore) {
		float min_score = MIN_VALUE;
		Position position;
        
		for(int i = 0 ;i < SMALL_MAP_SIZE ;i++ ) {
			for(int j = 0 ;j < SMALL_MAP_SIZE ;j++) {
				if(smallMapScore[i][j] > min_score) {
					min_score = smallMapScore[i][j];
					position.x = i;
					position.y = j;
				}
			}
		}
		
		return position;
	}

	//judge player stand at small map whether is biggest
	bool PlayerStandAtSmallMapBiggest(Position playerPos ,Position biggestMap) {
		int x = playerPos.x / SMALL_MAP_SIZE;
		int y = playerPos.y / SMALL_MAP_SIZE;
		if(x != biggestMap.x || y != biggestMap.y) {
			return false;
		}
        
		return true;
	}

	//get far biggest small map near target if player do not stand at biggest small map
	Position GetTarget(Position biggestMap ,Position playerPos) {
		int near_distance = MAX_VALUE;
		Position position;
		Position near_position;
		for(int i = 0 ;i < SMALL_MAP_SIZE ;i++) {
			for(int j = 0 ;j < SMALL_MAP_SIZE ;j++) {
				if(i==0 || i==4 || j==0 || j==4) {
					position.x = biggestMap.x*5+i;
					position.y = biggestMap.y*5+j;
					int distance = GetTwoCellDistance(position,playerPos);
					if(distance < near_distance) {
						near_distance = distance;
						near_position = position;
					}
				}
			}
		}
        
		return near_position;
	}

    Position Run(char map[25][25], vector<Position> enemyPos, Position playerPos) {
        vector<vector<float>> smallMapScore(5, vector<float>(5, 1.0));
        
    	smallMapScore = GetSmallMapScore(map ,playerPos ,enemyPos);
    	Position biggestMap = GetBiggestSmallMapPosition(smallMapScore);
        cout << biggestMap.x << "," << biggestMap.y << endl;

        Position target;
    	if(!PlayerStandAtSmallMapBiggest(playerPos ,biggestMap)) {
    		target = GetTarget(biggestMap,playerPos);
            cout << target.x << "," << target.y << endl;
            return target;
    	}
        
        return playerPos;
    }    
};

int main(int argc, char **argv)
{	
	PlayerAction playerAction;
	char map[25][25] = 
        {'1', '8', '8', '8', '8', '8', '8', '8', '8', '8','8', '8', '8', '8', '8', '8', '2', '8', '8', '1','1', '8', '3', '1', '8', 
        '2', '8', '9', '1', '1', '1', '1', '1', '1', '9', '7', '1', '1', '1', '1', '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
                     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
                     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
                     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
                     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
                     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
                     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
                     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
                     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
                     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
                     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
                     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
                     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
                     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
                     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
                     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
                     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
                     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
                     '1', '1', '1', '1', '1'};
    
	Position playerPos = {3, 11};
    vector<Position> enemyPos = {{20,2}, {21,8}, {9,3}, {16,8}, {21, 4}, {16,7}, {3,17}};

    vector<vector<float>> smallMapScore(5, vector<float>(5, 1.0));
	smallMapScore = playerAction.GetSmallMapScore(map ,playerPos ,enemyPos);
    
	Position biggest_small_map_position = playerAction.GetBiggestSmallMapPosition(smallMapScore);

    cout << biggest_small_map_position.x << "," << biggest_small_map_position.y << endl;

    Position target;
	if(!playerAction.PlayerStandAtSmallMapBiggest(playerPos ,biggest_small_map_position)) {
		target = playerAction.GetTarget(biggest_small_map_position,playerPos);
        cout << target.x << "," << target.y << endl;
	}

	return 0;
}



