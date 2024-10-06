#ifndef BULLET_H
#define BULLET_H
#include <QVector2D>
#include<QGraphicsPixmapItem>
#include<QObject>
#include"defense.h"
class Bullet: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    //Bullet(QVector2D pos, QVector2D dir, float spe);
    Bullet( bool autoshoot,QPoint fp = QPoint(0,0), bool power=false);
    //void draw();
    QPoint finalpoint;
    int distancex;
    int distancey;
    static int getkilled();
    static int getgems();
    static int getpowerups();
    static int getusedpowerups();
    static int powerups;
    static int usedpowerups;
    bool powerBoosted;
    void activatePowerBoost();
    void deactivatePowerBoost();
    Defence* DUnit;
    void setdefenseunit(Defence* d);


public slots:
    void move();
    //void setfp(QPoint fp);

signals:

    void markerCollected();

private:
    float speed;
    QVector2D direction;
    static int killed;
    static int gems;
    QVector2D position;
};

#endif // BULLET_H
