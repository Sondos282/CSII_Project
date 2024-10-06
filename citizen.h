 #ifndef CITIZEN_H
#define CITIZEN_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPixmap>
#include "fence.h"

class Citizen: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    int health;
    bool isMid;
    int **arr;
    int r;
    int c;
    int castleI;
    int castleJ;
    bool busy;
    float xpos;
    float ypos;
    bool alive;
    bool stopped;
    QTimer* delayfixing;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QString currentDirection;
    QTimer* movetimer;
    QTimer* collisionTimer;
    QPoint fp;
    Fence* infence;
    int castlecollision;
    int fenceindex;

public:
    Citizen(QGraphicsScene *inscene,QGraphicsView *inview,int rows, int cols, int** inarr);
    bool isalive();
    void setmovetimer(int x, int y, Fence* f);
    void setmovetimer(int x, int y);

    Fence* FindNearestFence(QVector<Fence*> destroyedfences, int CastleXpos, int CastleYpos, int DX,int DY);
    bool ispathblocked(int CastleXpos, int CastleYpos, int DX,int DY, int FX, int FY);
    int getfenceindex();
    float FindDistanceFromFence(Fence *inFence,int CastleXpos, int CastleYpos, int DX,int DY);
    // Citizen();
    void stop();
    void resume();


public slots:
    void die();
    void revive();
    int getx();
    int gety();
    //void moveCalculator(int row, int column, Fence *fence);
    bool moveUp();
    void move();
    bool moveDown();
    bool moveRight();
    bool moveLeft();
    void fix(Fence *infence);
    void gotoMid();
    bool isBusy();
    void checkCollisions();
    void setscene(QGraphicsScene* s);
};
#endif // CITIZEN_H
