#ifndef WANGBK_H_
#define WANGBK_H_
#include<memory.h>
#include<cstdlib>
#include<iostream>
#include<vector>

#define GHOST 'G'
#define APPLE '1'
#define ORANGE '2'
#define PINEAPPLE '3'
#define STRAWBERRY '4'
#define WATERMELON '5'
#define WALL '9'
#define MAXSTEPS 5
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define MAPSIZE_X 25
#define MAPSIZE_Y 25

using std::cout;
using std::endl;
using std::string;

const char directions[4] = {UP, DOWN, LEFT, RIGHT};

typedef struct Location{
    int x;
    int y;
    bool operator ==(const Location loc) const {
        return (x == loc.x && y == loc.y);
	}
}Loc;

typedef struct Evaluate{
    int earnings;
    double avrEarning;
    Location from;
    char direction;
    char oriAction;
    int steps;
}Eva;
#endif

class Algorithm {
public:
    bool Init(std::vector<string> map, int size, Location oriPos, char drct) {
        m_map = map;
        m_size = size;
        m_origin = oriPos;
        m_direction = drct;
        m_evaluate = reinterpret_cast<Evaluate**>(malloc(sizeof(Evaluate*)*size));

        for (int i = 0; i < size; i++) {
    	    m_evaluate[i] = (Evaluate*)malloc(sizeof(Evaluate)*size);
            for(int j = 0; j < size; j++) {
                m_evaluate[i][j] = {0, 0.0, {-1, -1}, 0, 0, 0};
            }
        }

        m_evaluate[m_origin.x][m_origin.y] = {0, 0.0, {-2,-2}, m_direction, 0, 0};
        m_maxValLoc = m_origin;

        return true;
    }

    void Run(Location location) {
        for (int i = 0; i < 4; i++) {
            int stepCost = 2;
            if(directions[i] == m_direction) {
                stepCost = 1;
            }
            CalcEvaByEarning(location, directions[i], directions[i], stepCost, 0);
        }
    }

    Location MoveableJudge(Location location, char drct) {
        int row_diff = 0, column_diff = 0;
        switch(drct) {
            case UP:
            	row_diff = -1;
            	break;
            case DOWN:
            	row_diff = 1;
            	break;
            case LEFT:
            	column_diff = -1;
            	break;
            case RIGHT:
            	column_diff = 1;
            	break;
        }

        if(location.x + row_diff < 0 || location.x + row_diff >= m_size ||
            location.y + column_diff < 0 || location.y + column_diff >= m_size)	{
            return location;	
        }

        if(WALL == m_map[location.x + row_diff][location.y + column_diff] ||
            'w' == m_map[location.x + row_diff][location.y + column_diff] ||
            's' == m_map[location.x + row_diff][location.y + column_diff] ||
            'a' == m_map[location.x + row_diff][location.y + column_diff] ||
            'd' == m_map[location.x + row_diff][location.y + column_diff] ||
            GHOST == m_map[location.x + row_diff][location.y + column_diff]) {

            return location;
        }

        location.x += row_diff;
        location.y += column_diff;

        return location;
    }

    void CalcEvaByEarning(Location prePos, char drct, char oriAct, int step, int earning){
        if(MAXSTEPS < step) {
            return;
        }

        Location curPos = MoveableJudge(prePos, drct);
        if(curPos == prePos) {
            return;
        }

        earning += (m_map[curPos.x][curPos.y] - '0');
        double average = earning * 1.0 / step;

        if (average > m_evaluate[m_maxValLoc.x][m_maxValLoc.y].avrEarning) {
        	m_maxValLoc = curPos;
            cout << "MAX: " << m_maxValLoc.x << "," << m_maxValLoc.y << ": " << average << oriAct << endl;
        }

        if (average > m_evaluate[curPos.x][curPos.y].avrEarning) {
            m_evaluate[curPos.x][curPos.y] = {earning, average, prePos, drct, oriAct, step};
        }

        for (int i = 0; i < 4; i++) {
    		int stepCost = 2;
    		if (directions[i] == drct) {
    			stepCost = 1;
    		}
    		CalcEvaByEarning(curPos, directions[i], oriAct, step+stepCost, earning);
        }
    }

    char Decision() {
        cout << "xy: " << m_maxValLoc.x << "," << m_maxValLoc.y << endl;

        free(m_evaluate);
        return m_evaluate[m_maxValLoc.x][m_maxValLoc.y].oriAction;
    }

private:
    int m_size = 0;
    std::vector<string> m_map;
    Location m_origin = {-1, -1};
    char m_direction = '\0';

    Evaluate **m_evaluate = NULL;
    Location m_maxValLoc = {-1, -1};
};
