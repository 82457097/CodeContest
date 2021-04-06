#include<list>
#include<cstdlib>
#include<iostream>

#define MAPSIZE_X 10
#define MAPSIZE_Y 10

using std::list;
using std::cout;
using std::endl;

struct Node {
    int x, y;
    int G, H, F;
    Node* parent;
    
    Node(int x, int y) :x(x), y(y) {
        G = 0;
        H = 0;
        F = 0;
        parent = nullptr;
    }
};
    
class CAStar {
public:
    CAStar(int map[MAPSIZE_X][MAPSIZE_Y], int mapxlen, int mapylen);
    list<Node*> GetPath(Node* Star, Node* End);
    
private:
    list<Node*> open;
    list<Node*> close;
    int (*map)[MAPSIZE_Y];
    int mapxlen;
    int mapylen;
    int GetG(Node* CurNode, Node* node);
    int GetH(Node* node, Node* End);
    int GetF(Node* node);
    Node* GetNodeInlist(const list<Node*> &List, Node* node);
    bool IsCanGo(Node* node);
    list<Node*> GetArrondNode(Node* CurNode);
    Node* GetMaxFNode();
    Node* FindPath(Node* End);
};
