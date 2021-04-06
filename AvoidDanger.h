#include<iostream>
#include<vector>
#include<string>

#define MAPSIZE_X 25
#define MAPSIZE_y 25
#define DANGER_DISTANCE 5

using std::vector;
using std::cout;
using std::endl;
using std::string;

typedef struct Point{
    int x;
    int y;
    bool operator ==(const Point pos) const {
        return (x == pos.x && y == pos.y);
	}
}Poi;

class CheckDanger {
public:
    bool Init(std::vector<string> map, Point myPos){
		m_pos = myPos;
        for(int i = 0; i < MAPSIZE_X; ++i) {
			for(int j = 0; j < MAPSIZE_y; ++j) {
				//check Ghost and player and save them in danger list
				if(map[i][j] == 'G' || map[i][j] == 'w' || map[i][j] == 's' || map[i][j] == 'a' || map[i][j] == 'd') {
					Point tmpPos;
					tmpPos.x = i;
					tmpPos.y = j;
					
					if(tmpPos == m_pos && CalDistance(tmpPos) > 5) {
						continue;
					}
					
					danger.push_back(tmpPos);
				}else if(map[i][j] == '^' || map[i][j] == '<' || map[i][j] == 'v' || map[i][j] == '>') { // check the bullet and save them in bullets list
					Point tmpPos;
					tmpPos.x = i;
					tmpPos.y = j;
                    
                    if(CalDistance(tmpPos) > 5) {
						continue;
					}
                    
					bullent.push_back(tmpPos);
				}
			}
		}
    }

    int CalDistance(Point dangerPos) {
        return abs(m_pos.x - dangerPos.x) + abs(m_pos.y - dangerPos.y);
    }

	bool InSameLine(Point dangerPos) {
		if((0 == m_pos.x - dangerPos.x) || (0 == m_pos.y - dangerPos.y)) {
			return true;
		} 
		
		return false;
	}
	
	bool CanAttack(Point dangerPos) {
		if('G' != m_map[dangerPos.x][dangerPos.y]) {
			return false;
		}

		int distance = CalDistance(dangerPos);
		char ghostDirt;

		if(0 == (m_pos.x - dangerPos.x) && (m_pos.y - dangerPos.y) > 0) {
			ghostDirt = 'a';
		} else if(0 == (m_pos.x - dangerPos.x) && (m_pos.y - dangerPos.y) < 0) {
			ghostDirt = 'd';
		} else if(0 == (m_pos.y - dangerPos.y) && (m_pos.x - dangerPos.x) > 0) {
			ghostDirt = 'w';
		} else {
			ghostDirt = 's';
		}
		
		int cost = 1;
		if(ghostDirt != m_map[m_pos.x][m_pos.y]) {
			cost++;
		}
		
		if(distance - cost >= 0) {
			return true;
		}
		
		return false;
	}
	
	bool IfNeedAvoidBullen(Point bulletPos){
		int distance = CalDistance(bulletPos);
		if(distance > 2){
			return false;
		}else{
			switch(m_map[bulletPos.x][bulletPos.y]){
    			case '^':
    				if(bulletPos.x <= m_pos.x){
    					return false;
    				}
    			    break;
                    
    			case '<':
        			if(bulletPos.y <= m_pos.y){
        				return false;
        			}
        			break;
                
    			case 'v':
        			if(bulletPos.x >= m_pos.x){
        				return false;
        			}
        			break;
                
    			case '>':
        			if(bulletPos.y >= m_pos.y){
        				return false;
        			}
        			break;
                
    			default:
    			    break;
			}
		}
        
		return true;
	}

    string HowToAvoid(Point danger) {
        if(danger.x == m_pos.x && danger.y > m_pos.y) {
            return "a";
        } else if(danger.x == m_pos.x && danger.y < m_pos.y) {
            return "d";
        }

        if(danger.y == m_pos.y && danger.x > m_pos.x) {
            return "w";
        } else if(danger.y == m_pos.y && danger.x < m_pos.x) {
            return "s";
        }

        if(danger.x > m_pos.x && danger.y > m_pos.y) {
            return "wa";
        } else if(danger.x > m_pos.x && danger.y < m_pos.y) {
            return "wd";
        }

        if(danger.x < m_pos.x && danger.y > m_pos.y) {
            return "sa";
        } else if(danger.x < m_pos.x && danger.y < m_pos.y) {
            return "sd";
        }
    }

    string HowToAttack(Point danger) {
        if(danger.x == m_pos.x && danger.y > m_pos.y) {
            if(m_map[m_pos.x][m_pos.y] == 'd') {
                return " v";
            } else {
                return "dv";
            }
        } else if(danger.x == m_pos.x && danger.y < m_pos.y) {
            if(m_map[m_pos.x][m_pos.y] == 'a') {
                return " v";
            } else {
                return "av";
            }
        }

        if(danger.y == m_pos.y && danger.x > m_pos.x) {
            if(m_map[m_pos.x][m_pos.y] == 's') {
                return " v";
            } else {
                return "sv";
            }
        } else if(danger.y == m_pos.y && danger.x < m_pos.x) {
            if(m_map[m_pos.x][m_pos.y] == 'w') {
                return " v";
            } else {
                return "wv";
            }
        }
    }
    
	string Run() {
		Point nearestDanger = {100, 100};
        if(!danger.size()) {
            for(auto v : danger) {
                nearestDanger = CalDistance(nearestDanger) < CalDistance(v) ? nearestDanger : v;
            }
        }

        Point nearestBullent = {100, 100};
        if(!bullent.size()) {
            for(auto v : bullent) {
                nearestBullent = CalDistance(nearestBullent) < CalDistance(v) ? nearestBullent : v;
            }
        }

        Point minDistance = CalDistance(nearestBullent) < CalDistance(nearestDanger) ? nearestBullent : nearestDanger;

        if(minDistance.x == 100) {
            return "1";
        }
        
		if(CalDistance(minDistance) <= 2 && !(minDistance == nearestBullent)){
            if(CanAttack(minDistance)) {
                return HowToAttack(minDistance);
            } else {
			    return HowToAvoid(minDistance);
            }
        } else if(CalDistance(minDistance) <= 2 && minDistance == nearestBullent) {
            if(IfNeedAvoidBullen(minDistance)) {
                return HowToAttack(minDistance);
            } else {
                return "1";
            }
        } else if(CalDistance(minDistance) > 2 && !(minDistance == nearestBullent)) {
            if(CanAttack(minDistance)) {
                return HowToAttack(minDistance);
            } else {
			    return HowToAvoid(minDistance);
            }
        } else {
            return "1";
        }
	}


private:
    std::vector<string> m_map;
	Point m_pos;
    std::vector<Point> danger;
    std::vector<Point> bullent;
};