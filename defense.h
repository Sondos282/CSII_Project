#ifndef DEFENSE_H
#define DEFENSE_H
#include<QMouseEvent>
#include <QGraphicsItem>
#include<QCursor>
#include<QPixmap>
class Defence: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Defence(int x, int y, QGraphicsScene * s, QString infile);
    int strength;
    int xpos;
    int ypos;
    bool powerBoost;
    QGraphicsScene* scene;
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    QTimer* shooter;
    int getx();
    int gety();
    void setpowerBoosted();


public slots:
    void shoot(int x, int y);
    //void startPowerBoost();

};

#endif // DEFENSE_H
