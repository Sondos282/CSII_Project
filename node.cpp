#include "node.h"

node::node(int jcoordinate, int icoordinate, int w)
{
    x = (icoordinate*(1000/24))+20.5;
    y = (jcoordinate*(800/14))+28.5;
    i = icoordinate;
    j = jcoordinate;
    weight = w;
}
void node::setweight(int w)
{
    weight =w;
}

void node::setnodefence(Fence* f)
{
    nodefence =f;
}

Fence* node::getnodefence()
{
    return nodefence;
}

bool node::isthisnodeAfence()
{
    if(weight>0)
        return true;
    return false;
}
float node::getx()
{
    return x;
}
float node::gety()
{
    return y;
}
int node::geti()
{
    return i;
}
int node::getj()
{
    return j;
}

