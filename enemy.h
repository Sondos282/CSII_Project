#ifndef ENEMY_H
#define ENEMY_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsScene>
#include <QList>    // List << element
#include <QPointF>  //F for floating numbers
#include <QTimer>
#include "fence.h"
#include "castle.h"
#include"node.h"
#include<vector>
#include"graph.h"
using namespace std;
class Enemy: public QObject , public QGraphicsPixmapItem
{ Q_OBJECT
public:
    Enemy(QPoint f, QGraphicsItem * parent=0);
    void rotateToPoint(QPointF p);
    int getHealth() const;
    void decreaseHealth(int amount);
    void setdestination();
    QTimer* collisionTimer;
    QString current;
    bool alive;
    qreal speed;
    void setspeed(qreal s);
    void sethealth(int h);
    vector<node*> path;
    void getcurrentnode();
    graph* EnemyGraph;
    void setgraph(graph* g);
    node* currentnode;
    node* nextnode;
    bool stopped;
    bool shot;


signals:
    void destroyed();

public slots:
    //void moveForward();
    void move();
    void checkCollisions();
    void move1();
    void move2();
    void move3();
    void move0();
    void breakFence(QString curr, Fence *inFence);
    void breakCastle(QString curr, Castle *Castle);
    void setpath(vector<node*> a);
    void stop();
    void resume();
private:
    QList<QPointF> points;
    QPointF destination;
    int pointIndex;
    int health;
    QTimer* moveTimer;
    int currentnodeindex;

};

#endif


// #ifndef ENEMY_H
// #define ENEMY_H
// #include <QGraphicsPixmapItem>
// #include <QObject>
// #include <QGraphicsScene>
// #include <QList>    // List << element
// #include <QPointF>  //F for floating numbers
// #include <QTimer>
// #include "fence.h"
// #include "castle.h"
// #include"node.h"
// #include<vector>
// #include "graph.h"
// #include <queue>
// using namespace std;
// class Enemy: public QObject , public QGraphicsPixmapItem
// { Q_OBJECT
// public:
//     Enemy(graph *inGraph, QPoint f, QGraphicsItem * parent=0);
//     void rotateToPoint(QPointF p);
//     int getHealth() const;
//     void decreaseHealth(int amount);
//     void die();
//     void setdestination();
//     QTimer* collisionTimer;
//     QString current;
//     bool alive;
//     qreal speed;
//     void setspeed(qreal s);
//     void sethealth(int h);
//     vector<node*> path;
//     graph *GAMEgraph;
//     queue<node*> movementQueue;
// signals:
//     void destroyed();

// public slots:
//     //void moveForward();
//     void move();
//     void checkCollisions();
//     void move1();
//     void move2();
//     void move3();
//     void move0();
//     void breakFence(QString curr, Fence *inFence);
//     void breakCastle(QString curr, Castle *Castle);
//     void setpath(vector<node*> a);
//     void CheckPathChanges();

// private:
//     QList<QPointF> points;
//     QPointF destination;
//     QPointF Finaldestination;
//     node* spawnNodeLocation;

//     int pointIndex;
//     int timeschangedpath;
//     int health;
//     QTimer* moveTimer;
//     QTimer* pathTimer;

//     int currentnodeindex;

// };

// #endif
