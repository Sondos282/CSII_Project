#ifndef NODE_H
#define NODE_H
#include "fence.h"
class node
{
    float x;
    float y;
    int i;
    int j;
public:
    node(int i, int j, int w);
    float getx();
    float gety();
    int geti();
    int getj();
    void setweight(int w);
    int weight;
    bool isthisnodeAfence();
    Fence* nodefence;
    void setnodefence(Fence* f);
    Fence* getnodefence();




};

#endif // NODE_H
