#ifndef CASTLE_H
#define CASTLE_H
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
class Castle: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Castle();
    int getx();
    int gety();
    void setposition(int x, int y, QGraphicsScene* s);
    qreal health;
    qreal xpos;
    qreal ypos;
    void decreasehealth();
    qreal gethealth();
    bool checkhealth();
    void sethealth(int x);
    QGraphicsScene* scene;
};

#endif // CASTLE_H
