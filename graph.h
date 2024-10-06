#ifndef GRAPH_H
#define GRAPH_H
#include"node.h"
#include"map"
#include"vector"
using namespace std;

class graph
{
    //map<node*, vector<pair<node*, double>>> adjacencylist;
public:
    graph();
    map<node*, vector<pair<node*, double>>> adjacencylist;

    void addedge(node* node1, node* node2, double cost);
    node* getnode(int i, int j);
    vector<node*> AStar(node* source);
    void addnode(int i, int j, int w);
    bool doesnodeexist(int i, int j);
    node* goalnode;
    void setgoal(int i, int j, int w);
    double heuristic(node* n);

};

#endif // GRAPH_H
