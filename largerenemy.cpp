#include "largerenemy.h"
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

largerEnemy::largerEnemy(QPoint f, QGraphicsItem* parent) : QGraphicsPixmapItem(parent) {

    health = 200;
    alive = true;
    speed = 2.45;

    setZValue(1);
    qreal x, y;
    //adjusting the initial position of the enemy
    int side = QRandomGenerator::global()->bounded(2); // Choose a random side (top, right, bottom, left)
    switch (side) {
    case 0: // Right side
        x = 900;
        y = QRandomGenerator::global()->bounded(300, 600);
        break;

    case 1: // Left side
        x = 0;
        y = QRandomGenerator::global()->bounded(300, 600);
        break;
    }

    setPos(x, y);
    collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &largerEnemy::checkCollisions);
    collisionTimer->start(100);
    destination =f;

}

void largerEnemy::rotateToPoint(QPointF p) {
    QLineF ln(pos(), p);
    setRotation(-1 * ln.angle());
}
//setting the destination
void largerEnemy::setdestination(QPoint f) {
    moveTimer = new QTimer;
    destination =f;
    connect(moveTimer, &QTimer::timeout, this, &largerEnemy::move);
    moveTimer->start(100);
}

//returning the health of the enemy
int largerEnemy::getHealth() const {
    return health;
}
//decreasing the health of the enemy
void largerEnemy::decreaseHealth(int amount) {
    health -= amount;
}

void largerEnemy::move1() {
    setPos(x()-1, y());
}
//checking collisions
void largerEnemy::checkCollisions()
{
    QList<QGraphicsItem*> colliding_Items = collidingItems();

    for (QGraphicsItem* item : colliding_Items)
    {

        if (Fence *fence = dynamic_cast<Fence*>(item))
        {

            if(fence->health>0)
            {
                moveTimer->stop();
                collisionTimer->stop();
                breakFence(current, fence);
                return;
            }

        }
        else if (Castle *castle = dynamic_cast<Castle*>(item))
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

void largerEnemy::move()
{
    QPointF direction = destination - pos();
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length > 0) {
        direction /= length;
    }

    if (qAbs(direction.x()) > qAbs(direction.y())) {
        if (direction.x() > 0) {
            QPixmap right(":/new/prefix1/largerEnemyRight.png");
            qreal scaleFactor = 0.25;
            this->setScale(scaleFactor);
            this->setPixmap(right);
            current = "right";
        }
        else {
            QPixmap left(":/new/prefix1/largerEnemyLeft.png");
            qreal scaleFactor = 0.25;
            this->setScale(scaleFactor);
            this->setPixmap(left);
            current = "left";

        }
    } else {
        if (direction.y() > 0) {
            QPixmap down(":/new/prefix1/largerEnemyDown.png");
            qreal scaleFactor = 0.25;
            this->setScale(scaleFactor);
            this->setPixmap(down);
            current = "down";

        } else {
            QPixmap up(":/new/prefix1/largerEnemyUp.png");
            qreal scaleFactor = 0.25;
            this->setScale(scaleFactor);
            this->setPixmap(up);
            current = "up";

        }
    }

    setPos(x() + speed * direction.x(), y() + speed * direction.y());


    qreal distanceToFinalPoint = QLineF(pos(), destination).length();
    if (distanceToFinalPoint <= speed) {
        setPos(destination);
    }
}
void largerEnemy::breakFence(QString curr, Fence *infence)
{
    QString imagePath;

    // Choose a single image path based on the current direction
    if(curr=="right")
    {
        imagePath = ":/new/prefix1/largerEnemyRight.png";
    }
    else if(curr=="left") {
        imagePath = ":/new/prefix1/largerEnemyLeft.png";
    }
    else if(curr=="up")
    {
        imagePath = ":/new/prefix1/largerEnemyUp.png";
    }
    else if(curr=="down")
    {
        imagePath = ":/new/prefix1/largerEnemyDown.png";
    }

    // Set the single image
    QPixmap pixmap(imagePath);
    qreal scaleFactor = 0.25;
    this->setScale(scaleFactor);
    this->setPixmap(pixmap);

    QTimer *timer = new QTimer(this);

    timer->setInterval(200);
    connect(timer, &QTimer::timeout, [=]() mutable {
        if(alive)
        {
            if (infence->health == 0 || infence->health<0)
            {
                timer->stop();
                disconnect(timer, &QTimer::timeout, nullptr, nullptr);
                timer->deleteLater();
                connect(moveTimer, &QTimer::timeout, this, &largerEnemy::move);
                moveTimer->start(100);
                collisionTimer->start(100);
                return;
            }
            infence->decreasehealth();
        }
    });
    timer->start();
}

void largerEnemy::breakCastle(QString curr, Castle *inCastle)
{
    QString imagePath;

    // Choose a single image path based on the current direction
    if(curr=="right")
    {
        imagePath = ":/new/prefix1/largerEnemyRight.png";
    }
    else if(curr=="left") {
        imagePath = ":/new/prefix1/largerEnemyLeft.png";
    }
    else if(curr=="up")
    {
        imagePath = ":/new/prefix1/largerEnemyUp.png";
    }
    else if(curr=="down")
    {
        imagePath = ":/new/prefix1/largerEnemyDown.png";
    }

    // Set the single image
    QPixmap pixmap(imagePath);
    qreal scaleFactor = 0.25;
    this->setScale(scaleFactor);
    this->setPixmap(pixmap);

    QTimer *timer = new QTimer(this);

    timer->setInterval(1000); // Set your desired interval
    connect(timer, &QTimer::timeout, [=]() mutable {
        if (inCastle->health <= 0)
        {
            timer->stop();
            disconnect(timer, &QTimer::timeout, nullptr, nullptr);
            timer->deleteLater();

            moveTimer->start(100); // Adjust your desired start interval
            collisionTimer->start(100); // Adjust your desired start interval
            return;
        }
        if(!alive)
            timer->stop();
        inCastle->decreasehealth();
    });

    timer->start();
}

void largerEnemy::setspeed(qreal s)
{
    speed= s;
}
void largerEnemy::sethealth(int h)
{
    health = h;
}
void largerEnemy::setpath(vector<node*> a)
{
    path =a;
}
