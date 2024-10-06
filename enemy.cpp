#include "enemy.h"
#include <QGraphicsScene>
#include <stdlib.h> // rand() -> to generate really large integer
#include <QTimer>
#include"fence.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QLabel>
#include <qmath.h>
#include <QList>
#include"defense.h"
#include"castle.h"
#include"citizen.h"
#include <QRandomGenerator>
#include<QMediaPlayer>
#include<QAudioOutput>
Enemy::Enemy(QPoint f, QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    //setting the data members of the enemy
    stopped = false;
    health = 100;
    alive = true;
    shot = false;
    speed = 2.45;
    QPixmap troops(":/new/prefix1/enemyDown.png");
    qreal scaleFactor = 0.6;
    this->setScale(scaleFactor);
    setPixmap(troops);
    setZValue(1);
    qreal x, y;
    //randomizing the side at which the enemy will show up
    int side = QRandomGenerator::global()->bounded(4); // Choose a random side (top, right, bottom, left)
    switch (side) {
    case 0: // Top side
        x = QRandomGenerator::global()->bounded(300, 700);
        y = 0;
        break;
    case 1: // Right side
        x = 900;
        y = QRandomGenerator::global()->bounded(300, 600);
        break;
    case 2: // Bottom side
        x = QRandomGenerator::global()->bounded(300, 700);
        y = 700;
        break;
    case 3: // Left side
        x = 0;
        y = QRandomGenerator::global()->bounded(300, 600);
        break;
    }

    setPos(x, y);
    collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &Enemy::checkCollisions);
    collisionTimer->start(100);

}
void Enemy::rotateToPoint(QPointF p)
{
    QLineF ln(pos(), p);
    setRotation(-1 * ln.angle());
}
void Enemy::setdestination()
{
    //setting the destination of the enemy as the nextnode in the path vector that represents the shortest path to the castle
    moveTimer = new QTimer;
    if(nextnode!=nullptr)
    {
    QPointF test (nextnode->getx(), nextnode->gety());
    destination = test;
    }
    connect(moveTimer, &QTimer::timeout, this, &Enemy::move);
    moveTimer->start(100);
}
int Enemy::getHealth() const
{
    return health;
}

void Enemy::decreaseHealth(int amount)
{
    health -= amount;
}

void Enemy::move0()
{
    setPos(x(), y()+1);

}
void Enemy::move1()
{
    setPos(x()-1, y());

}
void Enemy::move2()
{
    setPos(x(), y()-1);


}
void Enemy::move3()
{
    setPos(x()+1, y());

}
void Enemy::checkCollisions()
{
    QList<QGraphicsItem*> colliding_Items = collidingItems();

    for (QGraphicsItem* item : colliding_Items)
    {

        if (Castle *castle = dynamic_cast<Castle*>(item))
        {
            moveTimer->stop();
            collisionTimer->stop();
            breakCastle(current, castle);
            return;
        }
        else if (typeid(item) == typeid(Defence))
        {
            for(int i =0;i<10; i++)
                move1();
        }
        else if (Citizen *e = dynamic_cast<Citizen*>(item))
        {
            e->die();
        }
    }

}


void Enemy::move()
{
    if(!stopped)
    {
    //checks that there is a path to the castle and there is a node in that path
    if(nextnode!=nullptr)
    {
        QPointF test (nextnode->getx(), nextnode->gety());

    destination=test; //making the nextnode the destination of the enemy

    QPointF direction = destination - pos();
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length > 0) {
        direction /= length;
    }

    if (qAbs(direction.x()) > qAbs(direction.y())) {
        if (direction.x() > 0) {
            QPixmap right(":/new/prefix1/enemyRight.png");
            qreal scaleFactor = 0.6;
            this->setScale(scaleFactor);
            this->setPixmap(right);
            current = "right";
        }
        else {
            current = "left";

        }
    } else {
        if (direction.y() > 0) {
            QPixmap down(":/new/prefix1/enemyDown.png");
            qreal scaleFactor = 0.6;
            this->setScale(scaleFactor);
            this->setPixmap(down);
            current = "down";

        } else {
            QPixmap up(":/new/prefix1/enemyUp.png");
            qreal scaleFactor = 0.6;
            this->setScale(scaleFactor);
            this->setPixmap(up);
            current = "up";

        }
    }
    setPos(x() + speed * direction.x(), y() + speed * direction.y());


    qreal distanceToFinalPoint = QLineF(pos(), destination).length();
    //checks if the remaining distance is less than the speed
    if (distanceToFinalPoint <= speed) {
        setPos(destination);
        getcurrentnode();//calls the function to get the next node in the path
        if(currentnode->isthisnodeAfence()) //checks if the current node is a fence to break it
        {
            if(currentnode->getnodefence()->health>0) //checks that it is not already broken
            {
            moveTimer->stop();
            collisionTimer->stop();
            breakFence(current, currentnode->getnodefence());
            }
        }
    }
    }
    else //checks if there is no next node which means that the enemy's actual node is not defined yet so that it needs to move a little bit to be defined
    {
        setPos(x()+1, y()+1);
        getcurrentnode();
    }
    }

}
void Enemy::breakFence(QString curr, Fence *infence)
{
    QStringList imagePaths;
  if(curr=="right"||curr=="left")
    {
        imagePaths = {":/new/prefix1/895_0.png", ":/new/prefix1/896_0.png", ":/new/prefix1/897_0.png", ":/new/prefix1/898_0.png", ":/new/prefix1/901_0.png"};
    }
    else if(curr=="up")
    {
        imagePaths = {":/new/prefix1/908_0.png", ":/new/prefix1/909_0.png", ":/new/prefix1/911_0.png", ":/new/prefix1/914_0.png"};

    }
    else if(curr=="down")
    {
        imagePaths = {":/new/prefix1/884_0.png", ":/new/prefix1/885_0.png", ":/new/prefix1/886_0.png", ":/new/prefix1/887_0.png"};

    }
    QTimer *timer = new QTimer(this);
    int currentIndex = 0;

    timer->setInterval(200);
    connect(timer, &QTimer::timeout, [=]() mutable {
        if(alive&&!stopped)
        {
        //this makes the enemy stop when the fence is finally broken
        if (infence->health == 0 || infence->health<0)
        {
            timer->stop();
            disconnect(timer, &QTimer::timeout, nullptr, nullptr);
            timer->deleteLater();
            connect(moveTimer, &QTimer::timeout, this, &Enemy::move);
            moveTimer->start(100);
            collisionTimer->start(100);
            return;
        }
        //loops over the imageslist to handle animations
        if (currentIndex < imagePaths.size())
        {
            QPixmap pixmap(imagePaths[currentIndex]);
            this->setPixmap(pixmap);
            currentIndex++;
        }
        else
        {
            currentIndex = 0;
        }
        infence->decreasehealth();
        }
    });
    timer->start();
}

void Enemy::breakCastle(QString curr, Castle *inCastle)
{
    QStringList imagePaths;
    if(curr=="right"||curr=="left")
    {
        imagePaths = {":/new/prefix1/895_0.png", ":/new/prefix1/896_0.png", ":/new/prefix1/897_0.png", ":/new/prefix1/898_0.png", ":/new/prefix1/901_0.png"};
    }
    else if(curr=="up")
    {
        imagePaths = {":/new/prefix1/908_0.png", ":/new/prefix1/909_0.png", ":/new/prefix1/911_0.png", ":/new/prefix1/914_0.png"};

    }
    else if(curr=="down")
    {
        imagePaths = {":/new/prefix1/884_0.png", ":/new/prefix1/885_0.png", ":/new/prefix1/886_0.png", ":/new/prefix1/887_0.png"};

    }

    QTimer *timer = new QTimer(this);
    int currentIndex = 0;

    timer->setInterval(1000);//was1000
    connect(timer, &QTimer::timeout, [=]() mutable {
        if(!stopped)
            {
        if (inCastle->health <= 0)
        {
            timer->stop();
            disconnect(timer, &QTimer::timeout, nullptr, nullptr);
            timer->deleteLater();


            moveTimer->start(100);
            collisionTimer->start(100);
            return;
        }

        if (currentIndex < imagePaths.size())
        {
            QPixmap pixmap(imagePaths[currentIndex]);
            this->setPixmap(pixmap);
            currentIndex++;
        }
        else
        {
            currentIndex = 0;
        }
        if(!alive)
            timer->stop();
        inCastle->decreasehealth();
        }
    });
    timer->start();
}
void Enemy::setspeed(qreal s)
{
    speed= s;
}
void Enemy::sethealth(int h)
{
    health = h;
}
void Enemy::setpath(vector<node*> a)
{
    path =a;
}
void Enemy::setgraph(graph* g)
{
    EnemyGraph = g;
}

void Enemy::getcurrentnode()//returns the current node of the enemy, adjusts the shortest path, and gets the next node accordingly
{
    int icor = x()/41;
    int jcor = y()/57;
    //checks that the current position of the enemy is on a node
    if(EnemyGraph -> doesnodeexist(jcor, icor))
    {
        currentnode = EnemyGraph->getnode(jcor, icor);
        path = EnemyGraph->AStar(currentnode);
    }
    else if(EnemyGraph -> doesnodeexist(jcor+1, icor+1))
    {
        currentnode = EnemyGraph->getnode(jcor+1, icor+1);
        path = EnemyGraph->AStar(currentnode);
    }
    else if(EnemyGraph -> doesnodeexist(jcor-1, icor-1))
    {
        currentnode = EnemyGraph->getnode(jcor-1, icor-1);
        path = EnemyGraph->AStar(currentnode);
    }
    else if(EnemyGraph -> doesnodeexist(jcor+1, icor-1))
    {
        currentnode = EnemyGraph->getnode(jcor+1, icor-1);
        path = EnemyGraph->AStar(currentnode);


    }
    else if(EnemyGraph -> doesnodeexist(jcor-1, icor+1))
    {
        currentnode = EnemyGraph->getnode(icor-1, jcor+1);
        path = EnemyGraph->AStar(currentnode);

    }
    if(path.size()>1)
    {
        nextnode = path[1];

    }
    else
    {
        nextnode = nullptr;
    }


}

void Enemy::stop() {
    stopped=true;
}
void Enemy::resume() {
    stopped = false;
}

