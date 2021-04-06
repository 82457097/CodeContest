#include"wangbk.h"

//以下四个属性需要输入
int size = 0;                             //地图尺寸
std::vector<string> map;                  //地图
Location origin = {-1, -1};               //起始点
char direction = '\0';                    //起始方向

//得到一个评估矩阵
Evaluate **evaluate = NULL;               //评估矩阵
Location maxValLoc = {-1, -1};            //价值最高点


char init(std::vector<string> mp, int sz, Location ori, char drct);

//void run();							//开始计算

void disposal();					//整理地图数据

void calcEvaluateByEarning(Location from, char direc, char ori_act, int steps, int earning);

char decision();				//最终决策行动方向

char init(std::vector<string> mp, int sz, struct Location ori, char drct) {
    map = mp;
	size = sz;
	origin = ori;
	direction = drct;
	evaluate = (Evaluate**)malloc(sizeof(Evaluate*)*size);
	for(int i = 0; i < size; i++) {
		evaluate[i] = (Evaluate*)malloc(sizeof(Evaluate)*size);
		for(int j = 0; j < size; j++) {
			evaluate[i][j] = {0, 0.0, {-1, -1}, 0, 0, 0};
		}
	}
	evaluate[origin.row][origin.column] = {0, 0.0, {-2,-2}, direction, 0, 0};
	maxValLoc = origin;
}

int abstractDirection(char drct) {
	return ((10 - abs(drct - 100) % 10)/2 + 1)/2;
}

Location moveableJudge(Location location, char direc) {
	int row_diff = 0, column_diff = 0;
	switch(direc) {
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
    
	if(location.row + row_diff < 0 || location.row + row_diff >= size ||
		location.column + column_diff < 0 || location.column + column_diff >= size)	{	//out of border

        return location;	
	}
        
	if(WALL == map[location.row + row_diff][location.column + column_diff] || 
        'w' == map[location.row + row_diff][location.column + column_diff] ||
        's' == map[location.row + row_diff][location.column + column_diff] ||
        'a' == map[location.row + row_diff][location.column + column_diff] ||
        'd' == map[location.row + row_diff][location.column + column_diff] ||
        'G' ==map[location.row + row_diff][location.column + column_diff]) {
 
        return location;
	}
        
	location.row += row_diff;
	location.column += column_diff;
    
	return location;
}

void calcEvaluateByEarning(struct Location from, char direc, char ori_act, int steps, int earning) {
	if(MAXSTEPS < steps)
		return;
	Location current = moveableJudge(from, direc);
	if(current == from)
		return;
	
    earning += (map[current.row][current.column] - '0');
    double average = earning * 1.0 / steps;

    if (average > evaluate[maxValLoc.row][maxValLoc.column].average_earnings) {
		maxValLoc = current;
        cout << "MAX: " << maxValLoc.row << "," << maxValLoc.column << ": " << average << ori_act << endl;
	}

    if (average > evaluate[current.row][current.column].average_earnings) {
		evaluate[current.row][current.column] = {
			earning, average, from, direc, ori_act, steps
		};
	}
	
	for(int i = 0; i < 4; i++) {
		int step_cost = 2;
		if(directions[i] == direc) {
			step_cost = 1;
		}
		calcEvaluateByEarning(current, directions[i], ori_act, steps + step_cost, earning);
	}
	
}

char decision() {
    cout << "xy: " << maxValLoc.row << maxValLoc.column << evaluate[maxValLoc.row][maxValLoc.column].originaction << endl;
	return evaluate[maxValLoc.row][maxValLoc.column].originaction;
}


char _decision() {
	int row_begin = origin.row - MAXSTEPS >= 0 ? origin.row - MAXSTEPS : 0;
	int row_end = origin.row + MAXSTEPS <= size-1 ? origin.row + MAXSTEPS : size - 1;
	int column_begin = origin.column - MAXSTEPS >= 0 ? origin.column - MAXSTEPS : 0;
	int column_end = origin.column + MAXSTEPS <= size-1 ? origin.column + MAXSTEPS : size - 1;
	int[] actionscore = {0, 0, 0, 0};
	int[] totalsteps = {0, 0, 0, 0};
	for (int i = row_begin; i <= row_end; i++) {
		for(int j = column_begin; j <= column_end; j++) {
			if(evaluate[i][j].from == {-1, -1} || evaluate[i][j].from == {-2, -2})
				continue;
			int abstractdir = abstractDirection(evaluate[i][j].originaction);
			actionscore[abstractdir] += evaluate[i][j].earnings;
			totalsteps[abstractdir] += evaluate[i][j].steps;
		}
	}
	double max_average_score = 0.0;
	int action = 0;
	for (int i = 0; i < 4; i ++) {
		double temp = actionscore[i]/totalsteps[i];
		if (temp > max_average_score) {
			max_average_score = temp;
			action = i;
		}
	}
	
	return directions[action];
}*/

int main() {
    char map[10][10] = {
     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
     '1', '1', '1', '1', '1', '1', '2', '1', '1', '1',
     '1', '1', '3', '1', '1', '2', '1', '1', '1', '1',
     '1', '1', '1', '1', '9', '7', '1', '1', '1', '1',
     '1', '1', '4', '9', '1', '1', '9', '1', '9', '1',
     '1', '1', '1', '1', '2', '0', '9', '1', '1', '1',
     '1', '1', '1', '1', '1', '5', '1', '1', '1', '1',
     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1',
     '1', '1', '1', '9', '1', '1', '9', '1', '1', '1',
     '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'
    };
    Location location = {5, 5};
    init(map, 10, location, 'd');
    calcEvaluateByEarning(location, UP, UP, 2, 0);
    cout << "start Down" << endl;
    calcEvaluateByEarning(location, DOWN, DOWN, 2, 0);
    cout << "start Right" << endl;
    calcEvaluateByEarning(location, RIGHT, RIGHT, 1, 0);
    calcEvaluateByEarning(location, LEFT, LEFT, 2, 0);

    //cout << "max value: " << evaluate[6][5].average_earnings << endl;

    cout << "next action: " << decision() << endl;

    return 0;
}

