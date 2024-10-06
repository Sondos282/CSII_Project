
#include "graph.h"
#include<cmath>
#include <queue>
#include<algorithm>
#include<QDebug>
#include <set>
using namespace std;

graph::graph() {}

// Function to add an edge between two nodes with a specified cost
void graph::addedge(node* node1, node* node2, double cost)
{
    adjacencylist[node1].push_back({node2,cost});
}

// Function to add a node to the graph with specified coordinates and weight
void graph::addnode(int i, int j, int w)
{
    node* n = new node(i, j, w);
    adjacencylist.insert({n, {}});
}

// Function to set the goal node for pathfinding with specified coordinates and weight
void graph::setgoal(int i, int j, int w)
{
    goalnode = new node(i, j, w);
    adjacencylist.insert({goalnode, {}});
}

// Function to get a node from the graph based on its coordinates
node* graph::getnode(int i, int j)
{
    for (auto iterator = adjacencylist.begin(); iterator!= adjacencylist.end(); iterator++) {
        if (iterator->first!= nullptr && iterator->first->geti() == j && iterator->first->getj() == i) {
            return iterator->first;
        }
    }
    return nullptr;
}

// Function to check if a node exists in the graph based on its coordinates
bool graph::doesnodeexist(int i, int j)
{
    for (auto iterator = adjacencylist.begin(); iterator!= adjacencylist.end(); iterator++) {
        if (iterator->first!= nullptr && iterator->first->geti() == j && iterator->first->getj() == i) {
            return true;
        }
    }
    return false;
}

// Function to calculate the heuristic value (estimated cost) from a node to the goal node
double graph::heuristic(node* a)
{
    double h = sqrt(pow(a->getx() - goalnode->getx(), 2) + pow(a->gety() - goalnode->gety(), 2));
    return h / 24.5; // Returns the approximate time left to reach the goal (24.5 is the enemy's speed per second)
}

// Function to find the shortest path from the source node to the goal node using A* algorithm
vector<node*> graph::AStar(node* source)
{
    priority_queue<pair<double, node*>, vector<pair<double, node*>>, greater<pair<double, node*>>> openlist;
    set<node*> closedlist;
    map<node*, node*> camefrom;
    map<node*, double> gscore;
    map<node*, double> fscore;

    openlist.push(make_pair(0, source)); // Pushing the source node to the open list

    gscore[source] = 0; // Initializing the g-score of the source node

    fscore[source] = heuristic(source); // Initializing the f-score of the source node

    while (!openlist.empty()) {
        node* currentnode = openlist.top().second; // Getting the node with the lowest f-score from the open list
        openlist.pop(); // Removing the current node from the open list

        if (currentnode->geti() == goalnode->geti() && currentnode->getj() == goalnode->getj())
        {
            // If the current node is the goal node, reconstruct and return the path
            vector<node*> path;
            node* temp = currentnode;
            while (temp != nullptr)
            {
                path.push_back(temp); // Adding nodes to the path
                temp = camefrom[temp];
            }
            reverse(path.begin(), path.end()); // Reversing the path to get the correct order
            return path;
        }

        closedlist.insert(currentnode); // Adding the current node to the closed list

        for (auto& neighbor : adjacencylist[currentnode])
        {
            // Iterating through the neighbors of the current node
            node* neighborNode = neighbor.first;
            double weight = neighbor.second; // Getting the weight of the edge

            if (closedlist.find(neighborNode) != closedlist.end())
            {
                // If the neighbor is in the closed list, skip it
                continue;
            }

            int tentativeGScore = gscore[currentnode] + weight + neighborNode->weight; // Calculating the tentative g-score

            if (gscore.find(neighborNode) == gscore.end() || tentativeGScore < gscore[neighborNode])
            {
                // If the tentative g-score is better than the current g-score of the neighbor
                camefrom[neighborNode] = currentnode; // Update the parent node
                gscore[neighborNode] = tentativeGScore; // Update the g-score
                fscore[neighborNode] = tentativeGScore + heuristic(neighborNode); // Update the f-score
                openlist.push(make_pair(fscore[neighborNode], neighborNode)); // Add the neighbor to the open list
            }
        }
    }
    return {}; // Return an empty vector if no path is found
}
