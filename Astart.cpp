#include"Astart.h"

CAStar::CAStar(int map[MAPSIZE_X][MAPSIZE_Y], int mapxlen, int mapylen) {
    this->map = map;
    this->mapxlen = mapxlen;
    this->mapylen = mapylen;
}

list<Node*> CAStar::GetPath(Node* Star, Node* End) {
    list<Node*> Path;
    open.push_back(Star);
    Node* result = FindPath(End);
    
    while (result) {
        Path.push_front(result);
        result = result->parent;
    }
    open.clear();
    close.clear();
    
    return Path;
}

int CAStar::GetG(Node* CurNode, Node* node) {
    int ThisG = map[node->x-1][node->y-1] - (abs(CurNode->x - node->x) + abs(CurNode->y - node->y));
    int parentG = node->parent == nullptr ? 0 : node->parent->G;
    
    return ThisG + parentG;
}

int CAStar::GetH(Node* node, Node* End) {
    return abs(End->x - node->x) + abs(End->y - node->y);
}

int CAStar::GetF(Node* node) {
    return node->G - node->H;
}

list<Node*> CAStar::GetArrondNode(Node* CurNode) {
    int x[4] = { 0, 0, -1, 1};
    int y[4] = { -1, 1, 0, 0};
    list<Node*> List;
    
    for (int i = 0; i < 4; i++) {
        if (IsCanGo(new Node(CurNode->x + x[i], CurNode->y + y[i]))) {
            List.push_back(new Node(CurNode->x + x[i], CurNode->y + y[i]));
        }
    }
    
    return List;
}
Node* CAStar::GetMaxFNode() {
    Node* node = open.front();
    
    for (Node* var : open) {
        if (node->F < var->F) {
            node = var;
        }
    }

    //cout << "node val:" << map[node->x-1][node->y-1] << endl;
    return node;
}

Node* CAStar::GetNodeInlist(const list<Node *> &List, Node *node) {
    for(Node* tmpNode : List) {
        if(tmpNode->x == node->x && tmpNode->y == node->y) {
            return nullptr;
        }
    }

    return node;
}

bool CAStar::IsCanGo(Node *node) {
    if(node->x > mapxlen || node->y > mapylen
        || node->x < 1 || node->y < 1) {
        return false;
    }

    return true;
}

Node* CAStar::FindPath(Node* End) {
    while (!open.empty()) {
        Node* CurNode = GetMaxFNode();
        if(CurNode->x == End->x && CurNode->y == End->y) {
            return CurNode;
        }
        open.remove(CurNode);
        close.push_back(CurNode);
        list<Node*> List = GetArrondNode(CurNode);
        
        for (Node* node : List) {
            if (GetNodeInlist(open, node) && GetNodeInlist(close, node)) {
                node->parent = CurNode;
                node->G = GetG(CurNode, node);
                node->H = GetH(node, End);
                node->F = GetF(node);
                open.push_back(node);
            } else {
                int tempG = GetG(CurNode, node);
                if (node->G > tempG) {
                    node->parent = CurNode;
                    node->G = tempG;
                    node->F = GetF(node);
                }
            }
        }
    }
    
    return nullptr;
}
