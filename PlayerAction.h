#ifndef PLAYERACTION_H_
#define PLAYERACTION_H_
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>

using std::vector;
using std::cout;
using std::endl;
using std::string;

#define MAX_VALUE 999
#define MIN_VALUE -999
#define APPLE_VALUE 1
#define ORANGE_VALUE 2
#define PINEAPPLE_VALUE 3
#define STRAWBERRY_VALUE 4
#define WATERMELON_VALUE 5
#define WALL_VALUE -4
#define WALL '9'
#define MAPSIZE_X 25
#define MAPSIZE_Y 25
#define SMALL_MAP_SIZE 5

typedef struct Position {
    int x;
    int y;
}Pos;
#endif  // PLAYERACTION_H_"

class PlayerAction {
public:
    bool Init(Position playerPos, vector<Position> enemyPos) {
        m_smallMapScore = vector<vector<int>>(5, vector<int>(5, 1));
        m_playerPos = playerPos;
        m_enemyPos = enemyPos;

        return true;
    }

    int GetTwoCellDistance(Position cell1, Position cell2) {
        return abs(cell1.x - cell2.x) + abs(cell1.y - cell2.y);
    }

    // Compare the distance between fruit and players and enemies
    int GetFruitActualScore(Position fruitPos, int fruitScore) {
        int distToPlayer = GetTwoCellDistance(fruitPos, m_playerPos);
        if (distToPlayer == 0) {
            return 0;
        }

        int distToEnemy;
        for (int i = 0; i < m_enemyPos.size(); i++) {
            distToEnemy += GetTwoCellDistance(fruitPos, m_enemyPos[i]);
        }

        return (1 - (distToPlayer/(distToEnemy + distToPlayer))) * fruitScore;
    }

    void GetSmallMapScore(vector<string> map) {
        for (int i = 0; i < MAPSIZE_X; i++) {
            for (int j = 0; j < MAPSIZE_Y; j++) {
                Position position;
                position.x = j;
                position.y = i;
                if (WALL == map[i][j]) {
                    m_smallMapScore[i/SMALL_MAP_SIZE][j/SMALL_MAP_SIZE] += WALL_VALUE;
                } else {
                    m_smallMapScore[i/SMALL_MAP_SIZE][j/SMALL_MAP_SIZE]
                        += GetFruitActualScore(position, static_cast<int>(map[i][j] - '0'));
                }
            }
        }
    }

	// return biggest small map position
    Position GetBiggestSmallMapPosition() {
        int min_score = MIN_VALUE;
        Position position;

        for (int i = 0 ;i < SMALL_MAP_SIZE ;i++ ) {
            for (int j = 0 ;j < SMALL_MAP_SIZE ;j++) {
      		    if (m_smallMapScore[i][j] > min_score) {
                    min_score = m_smallMapScore[i][j];
                    position.x = i;
                    position.y = j;
                }
            }
        }

        return position;
    }

    // judge player stand at small map whether is biggest
    bool PlayerStandAtSmallMapBiggest(Position biggestMap) {
        int x = m_playerPos.x / SMALL_MAP_SIZE;
        int y = m_playerPos.y / SMALL_MAP_SIZE;
        if (x != biggestMap.x || y != biggestMap.y) {
            return false;
        }

        return true;
    }

    // get far biggest small map near target if player do not stand at biggest small map
    Position GetTarget(Position biggestMap) {
        int near_distance = MAX_VALUE;
        Position position;
        Position near_position;
        for (int i = 0 ;i < SMALL_MAP_SIZE ;i++) {
            for (int j = 0 ;j < SMALL_MAP_SIZE ;j++) {
                if (i == 0 || i == 4 || j == 0 || j == 4) {
                    position.x = biggestMap.x*5+i;
                    position.y = biggestMap.y*5+j;
                    int distance = GetTwoCellDistance(position, m_playerPos);
                    if (distance < near_distance) {
                        near_distance = distance;
                        near_position = position;
                    }
                }
            }
        }

        return near_position;
    }

    Position Run(vector<string> map) {
        GetSmallMapScore(map);
        Position biggestMap = GetBiggestSmallMapPosition();
        cout << biggestMap.x << "," << biggestMap.y << endl;

        Position target;
        if(!PlayerStandAtSmallMapBiggest(biggestMap)) {
            target = GetTarget(biggestMap);
            cout << target.x << "," << target.y << endl;
            return target;
        }

        return m_playerPos;
    }

private:
    Position m_playerPos;
    vector<Position> m_enemyPos;
    vector<vector<int>> m_smallMapScore;
};

