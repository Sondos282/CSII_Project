#ifndef LEVEL_H
#define LEVEL_H
#include<QGraphicsview>
#include<QMouseEvent>
#include<QWidget>
#include<QGraphicsPixmapItem>
#include"citizen.h"
#include<QTimer>
#include"winning.h"
#include <QQueue>
#include"castle.h"
#include"defense.h"
#include"graph.h"
#include"enemy.h"
#include"pausewindow.h"
class Level: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Level(QString location, QString infile, int d, bool inAimAssist);
    QTimer* citizentimer;
    QTimer* markerGenerator;
    bool AimAssist;

    void loadmap(const QString &location);
    QGraphicsScene *scene;
    QGraphicsView view;
    QGraphicsPixmapItem Item[14][24];
    QGraphicsScene* getscene();
    QGraphicsView* getview();
    void addanitem(QGraphicsPixmapItem* a);
    void addbackgroundmusic();
    int **Board;
    void endthegame();
    QTimer* leveltimer;
    winning* w;
    QTimer enemyTimer;
    QTimer* gemsgenerator;
    Citizen **citizens;
    QVector<Fence*> FencesDestroyed;
    Fence **Fences;
    int nooffences;
    QTimer* fencechecker;
    QTimer* powerupgenerator;
    QString defensefile;
    QString mapfile;
    Castle gamecastle;
    QTimer* gameover;
    QTimer* castlehealth;
    QTimer*Focuschecker;
    Defence *d1;
    bool revived;
    void addBrokenFence(Fence *inFence);
    void pause();
    void cont();
    Castle* getcastle();
    void setrevived();
    QTimer* delaycont;
    QGraphicsPixmapItem castlehealthimage;
    QGraphicsPixmapItem timeritem;
    QGraphicsTextItem* elapsedtime;
    QTimer* elapsedtimer;
    int elapsedt;
    int difficulty;
    int getdifficulty();
    int nofcitizens;
    QString getmap();
    QString getdefence();
    bool findNearestCitizen(Fence *inFence);
    //graph
    graph gamegraph;
    QTimer* largerEnemyGenerator;
    QPushButton *pauseButton;
    pauseWindow* pw;
    QList<Enemy*> enemies; // Use QList to manage multiple enemies
    void stopEnemies(); // Stop all enemies
    void resumeEnemies(); // Resume all enemies
    QTimer *AutoShooter;


public slots:
    void createEnemy();
    void checkFences();
    void generatepowerup();
    void isgameover();
    void generategems();
    void updatecastlehealth();
    void setFocusonDefense();
    void updateelapsedtime();
    void earthquake();
    void generateMarker();
    void createLargerEnemy();
    void pauseGame();
    void resumeGame();
    void endGame();
    void AutoShoot();
};

#endif // LEVEL_H
