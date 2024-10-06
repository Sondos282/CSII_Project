#ifndef FENCE_H
#define FENCE_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPixmap>
#include<QTimer>
class Fence: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    int health;
    int weight;
    bool broken;
    QGraphicsScene *scene;
    QGraphicsView *view;
    int i;
    bool assigned;
    int j;
    void sethealth(int h);


public:
    Fence(QGraphicsScene *inscene,QGraphicsView *inview, int i, int j, int c, int r);
    int geti();
    int getj();
    int getx();
    int gety();
    bool isbroken();
    void setfixed();
    bool fullHealth();
public slots:
    void incrementHealth();
    void fenceDesign();
    void decreasehealth();

};
#endif // FENCE_H
