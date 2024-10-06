#ifndef LARGERENEMY_H
#define LARGERENEMY_H

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
using namespace std;

class largerEnemy: public QObject , public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    largerEnemy(QPoint f, QGraphicsItem * parent=0);
    void rotateToPoint(QPointF p);
    int getHealth() const;
    void decreaseHealth(int amount);
    void setdestination(QPoint f);
    QTimer* collisionTimer;
    QString current;
    bool alive;
    qreal speed;
    void setspeed(qreal s);
    void sethealth(int h);
    vector<node*> path;

signals:
    void destroyed();

public slots:
    //void moveForward();
    void move();
    void checkCollisions();
    void move1();
    void breakFence(QString curr, Fence *inFence);
    void breakCastle(QString curr, Castle *Castle);
    void setpath(vector<node*> a);

private:
    QList<QPointF> points;
    QPointF destination;
    int pointIndex;
    int health;
    QTimer* moveTimer;
    int currentnodeindex;

};

#endif // LARGERENEMY_H
