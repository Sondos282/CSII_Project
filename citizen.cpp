#include "citizen.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRandomGenerator>
#include <cmath>
#include<QUrl>
#include<QMediaPlayer>
#include<QtGlobal>
#include<QAudioOutput>
#include <QTimer>
#include"castle.h"
#include"defense.h"
#include <QEventLoop>
#include<cmath>
using namespace std;
Citizen::Citizen( QGraphicsScene *inscene,QGraphicsView *inview,int rows, int cols, int** inarr): QObject() {
//adjusting citizen's data members
    health = 100;
    isMid = false;
    stopped = false;
    castlecollision=0;
    setPixmap(QPixmap(":/new/prefix1/1123_0.png"));
    r=rows;
    infence = nullptr;
//Connecting the movetimer
    movetimer = new QTimer;
    connect(movetimer, &QTimer::timeout, this, &Citizen::move);
    c=cols;
    alive = true;
    busy = false;
//Adjusting the Zvalue so that the enemy looks higher than other objects in the scene
    setZValue(1);
    delayfixing = new QTimer;
    currentDirection="NULL";
    collisionTimer = new QTimer(this);
    connect(collisionTimer, &QTimer::timeout, this, &Citizen::checkCollisions);
    collisionTimer->start(100);

//adjusting the rest position of the citizen
    arr = new int* [rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
    }
    for(int i=0; i<14; i++)
    {
        for(int j=0; j<24; j++)
        {
            arr[i][j]=inarr[i][j];
            if(arr[i][j]==1)
            {
                int randomnum = QRandomGenerator::global()->bounded(1,3);
                castleI = i+randomnum;
                castleJ = j+randomnum;
            }
        }
    }


    qreal scaleFactor = 0.6;
    this->setScale(scaleFactor);
    xpos =x();
    ypos =y();
    scene = inscene;
    view = inview;
    scene->addItem(this);
}
void Citizen::setmovetimer(int x, int y, Fence* f)
{

    QPoint a(x,y);
    infence =f;
    fp=a;
    busy = true;
    movetimer->start(100);
}
int Citizen::getx()
{
    return xpos;
}
int Citizen::gety()
{
    return ypos;
}
void Citizen::setscene(QGraphicsScene* s)
{
    scene = s;
}
void Citizen::die()
{
    QPixmap image(":/new/prefix1/255_1.png");
    setPixmap(image);
    setZValue(0);
    setPos(x(), y());
    if(infence!=nullptr)
        infence->decreasehealth();
    alive = false;
}
void Citizen::revive() //getting the citizen back alive
{
    alive = true;
    setPixmap(QPixmap(":/new/prefix1/1123_0.png"));
    setZValue(1);
    setPos(x(), y());
    busy = false;

}
void Citizen::setmovetimer(int x, int y)
{

    QPoint a(x,y);
    fp=a;
    busy = false;
    movetimer->start(100);
}



void Citizen::gotoMid() //A function that makes the citizen move to its rest position
{
    if(alive)
    {
    int inx = castleI * (1000/c);


    int iny = (castleJ * (800/r))-400;

    isMid = true;
    setmovetimer(inx, iny);
    }

}


void Citizen::move()
{
    if(alive&&!stopped)
    {
        setPixmap(QPixmap(":/new/prefix1/1123_0.png"));
        QPointF finalpoint = fp;
        QPointF direction = finalpoint - pos();

        qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

        if (length > 0)
        {
            direction /= length;
        }
        int speed = 7;
        qreal distanceToMove;
        // Move either the speed or the remaining distance, whichever is smaller
        if(length<speed)
        {
            distanceToMove= length;
        }
        else
            distanceToMove = speed;
        setPos(x() + distanceToMove * direction.x(), y() + distanceToMove * direction.y());

        qreal distanceToFinalPoint = QLineF(pos(), finalpoint).length();
        if (distanceToFinalPoint <= speed) {
            setPos(finalpoint);
            xpos = finalpoint.x();
            ypos = finalpoint.y();
            movetimer->stop();
            if(!isMid)
            {
                fix(infence);
            }
            else
            {
                isMid = false;
            }        }
    }
    else
        movetimer->stop();
}

//Checks if the citizen is busy
bool Citizen::isBusy()
{
    return busy;
}
bool Citizen::moveDown()
{
    if(!stopped)
    {
        setPixmap(QPixmap(":/new/prefix1/1114_0.png"));
        setPos(x(),y()+(800/r));
        return true;
    }
    return false;
}
bool Citizen::moveUp()
{
    if(!stopped)
    {
        setPixmap(QPixmap(":/new/prefix1/1158_0.png"));
        setPos(x(),y()-(800/r));
        return true;
    }
    return false;
}
bool Citizen::moveRight()
{
    if(!stopped)
    {
        setPixmap(QPixmap(":/new/prefix1/1123_0.png"));
        setPos(x()+(1000/c),y());
        return true;
    }
    return false;
}
bool Citizen::moveLeft()
{
    if(!stopped)
    {
        setPixmap(QPixmap(":/new/prefix1/1114_0.png"));
        setPos(x()-(1000/c),y());
        return true;
    }
    return false;
}
bool Citizen::isalive()
{
    return alive;
}
void Citizen::fix(Fence *infence)
{

    QStringList imagePaths = {":/new/prefix1/1111_0.png", ":/new/prefix1/1112_0.png", ":/new/prefix1/1113_0.png"};
    QTimer *timer = new QTimer(this);
    int currentIndex = 0;
    busy=true;
    infence->setfixed();


    timer->setInterval(200);
    connect(timer, &QTimer::timeout, [=]() mutable{
        if(alive)
        {
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
            infence->incrementHealth();

            if (infence->fullHealth())
            {
                timer->stop();
                disconnect(timer, &QTimer::timeout, nullptr, nullptr);
                infence=nullptr;
                timer->deleteLater();
                gotoMid();
            }
        }
    });

    timer->start();
    //adjust the time needed for the citizen to fix a fence so that it is more realistic
    delayfixing->setSingleShot(true); // this line makes the timer fires only once
    delayfixing->start(1000);
    QObject::connect(delayfixing, &QTimer::timeout, [=]()
                     {
                         busy = false;
                     });

}

void Citizen::checkCollisions()
{
    QList<QGraphicsItem*> colliding_Items = collidingItems();

    for (QGraphicsItem* item : colliding_Items)
    {
        //this ensures that the citizen doesn't cross the castle but pass besides it,
        //however, in some cases the citizen can go in an infinite loop while trying to pass besides the castle
        //that is why there is an integer called castlecollision that checks if it collided with the castle for more than 5
        //times. If so, it passes over it since there is no other option.
        if (Castle *castle = dynamic_cast<Castle*>(item))
        {
            castlecollision++;
            if(castlecollision<6)
            {
            if(castle->getx() > x() + 10 && currentDirection != "Right")
            {
                currentDirection = "Right";
                moveUp();
            }
            if(castle->getx() + 10 < x() && currentDirection != "Left")
            {
                currentDirection = "Left";
                moveUp();
            }
            if(castle->gety() > y() + 10 && currentDirection != "Down")
            {
                currentDirection = "Down";
                moveRight();
            }
            if(castle->gety() + 10 < y() && currentDirection != "Up")
            {
                currentDirection = "Up";
                moveRight();
            }
            }
            else
            {
                castlecollision =0;
                collisionTimer->stop();
                QTimer::singleShot(3000,this,[=]()
                                   {
                    collisionTimer->start();
                });
            }
        }
        if (Defence *defence = dynamic_cast<Defence*>(item))
        {
            if(defence->getx() > x() + 10 && currentDirection != "Right")
            {
                currentDirection = "Right";
                moveUp();
            }
            else if(defence->getx() + 10 < x() && currentDirection != "Left")
            {
                currentDirection = "Left";
                moveUp();
            }
            else if(defence->gety() > y() + 10 && currentDirection != "Down")
            {
                currentDirection = "Down";
                moveRight();
            }
            else if(defence->gety() + 10 < y() && currentDirection != "Up")
            {
                currentDirection = "Up";
                moveRight();
            }
        }
    }
}
bool Citizen::ispathblocked(int CastleXpos, int CastleYpos, int DX,int DY, int FX, int FY)
{
    //c -> castle, d->defence, LT is the left-top corner, RT is the right top one, RL is the right-lower corner and so on
    //this function checks if the shortest path between the citizen and the fence intersects with the bounding rectangle of the castle or the defense or not
    QPointF clt(CastleXpos, CastleYpos);
    QPointF crt(CastleXpos+120, CastleYpos);
    QPointF cll(CastleXpos, CastleYpos+120);
    QPointF crl(CastleXpos+120, CastleYpos+120);
    QPointF dlt(DX, DY);
    QPointF drt(DX+100, DY);
    QPointF dll(DX, DY+100);
    QPointF drl(DX+100, DY+100);

    QLineF ctop(clt, crt);
    QLineF cbottom(cll, crl);
    QLineF cleft(clt, cll);
    QLineF cright(crt, crl);

    QLineF dtop(dlt, drt);
    QLineF dbottom(dll, drl);
    QLineF dleft(dlt, dll);
    QLineF dright(drt, drl);

    QPointF initial(x(), y());
    QPointF final(FX, FY);
    QLineF path(initial, final);
    //the intersection valuse must be 1 for bounded intersection
    if(path.intersects(ctop)==1||path.intersects(cbottom)==1||path.intersects(cleft)==1||path.intersects(cright)==1||path.intersects(dtop)==1||path.intersects(dbottom)==1||path.intersects(dleft)==1||path.intersects(dright)==1) //the intersection function returns 2 if the intersection is unbounded
        return true;
    else
        return false;
}
//returns the nearest broken fence to that citizen
Fence* Citizen::FindNearestFence(QVector<Fence*> destroyedfences, int CastleXpos, int CastleYpos, int DX,int DY)
{
    Fence* nearestfence=nullptr;
    double minDistance = 10000;
    for(int i =0; i<destroyedfences.size(); i++)
    {
        int xp = destroyedfences[i]->gety();
        int yp = destroyedfences[i]->getx();
        double distance = sqrt(pow(xp-x(), 2) + pow(yp-y(), 2));
        if(ispathblocked(CastleXpos, CastleYpos,DX,DY, xp, yp))
        {
            distance = distance + 3*7; //the maximum time is 3 seconds and the citizen moves 7 units per second
        }
        if(distance<minDistance)
        {
            minDistance = distance;
            fenceindex = i;
        }
    }
    return destroyedfences[fenceindex];
}

float Citizen::FindDistanceFromFence(Fence *inFence,int CastleXpos, int CastleYpos, int DX,int DY)
{

    int xp = inFence->gety();
    int yp = inFence->getx();
    double distance = sqrt(pow(xp-x(), 2) + pow(yp-y(), 2));
    if(ispathblocked(CastleXpos, CastleYpos,DX,DY, xp, yp))
    {
        distance = distance + 3*7;
    }

    return distance;
}
//returns the index of the nearest fence in the DestroyedFences vectron in the level class
int Citizen::getfenceindex()
{
    return fenceindex;
}
void Citizen::stop() {

    stopped = true;
    collisionTimer->stop();
}

void Citizen::resume() {

    collisionTimer->start();
    stopped = false;
}
