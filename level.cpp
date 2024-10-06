#include "level.h"
#include <QApplication>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPixmap>
#include <QMediaPlayer>
#include <QAudioOutput>
#include<QUrl>
#include<QTimer>
#include"citizen.h"
#include"winning.h"
#include<QGraphicsPixmapItem>
#include<QFile>
#include<QTextStream>
#include<QCursor>
#include"defense.h"
#include"bullet.h"
#include<QGraphicsPixmapItem>
#include<QVector2D>
#include"fence.h"
#include"gameover.h"
#include"castle.h"
#include"enemy.h"
#include"gem.h"
#include"powerup.h"
#include<QRandomGenerator>
#include"healthmarkers.h"
#include<QPushButton>
#include<QGraphicsProxyWidget>
#include"largerenemy.h"
Level::Level(QString location, QString infile, int d, bool inAim) {

    difficulty = d; //adjusting the difficulty of the game on a scale of 1-5
    AimAssist = inAim; //saving the player's choice os game difficulty(Autoshooter or not)
    mapfile = location; //saving the level's map file
    nofcitizens = 5-((difficulty-1)*0.5); //setting the number of citizens based on the difficulty, 1->5, 2->4, 3->4, 4->3, 5->3;
    view.setFixedSize(1000, 800);
    defensefile = infile;//saving th level's defense type(file)
    view.setWindowTitle("Clash of Clans");
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,1000,800);
    view.setScene(scene);
    view.show();
    //A timer that checks if the castle's health is zero or not yet
    gameover = new QTimer;
    connect(gameover, &QTimer::timeout, this, &Level::isgameover);
    gameover->start(100);
    //A timer that counts down the left time for the game to end so that the player wins
    leveltimer = new QTimer();
    //A timer that generate enemies with a frequency that depend on the game difficulty
    citizentimer = new QTimer();
    citizentimer->setInterval(200);

    //checks if the player need an autoshooter or not
    if(AimAssist)
    {
        //if yes, a timer is connected to the autoShoot function
        AutoShooter = new QTimer();
        AutoShooter->setInterval(900);
        connect(AutoShooter, &QTimer::timeout, this , &Level::AutoShoot);
        AutoShooter->start();
    }

    w = new winning();

    //setting the map of the level based on the mapfile
    loadmap(location);
    //setting the value of revived boolean so that when a plalyer continues a game after losing he is unable to continue again
    revived=false;
    //timer to generate gems
    gemsgenerator = new QTimer();
    connect(gemsgenerator, &QTimer::timeout, this, &Level::generategems);
    gemsgenerator->start(6000);
    //timer to check if any fence is broken so that it assigns a citizen to fix it
    fencechecker = new QTimer;
    connect(fencechecker, SIGNAL(timeout()), this, SLOT(checkFences()));
    fencechecker->start(2000);
    //timer to generate the power upthe bring citizens back alive
    powerupgenerator=new QTimer;
    connect(powerupgenerator, SIGNAL(timeout()), this, SLOT(generatepowerup()));
    powerupgenerator->start(12000);
    //a timer to regularly check update the castle's health bar
    castlehealth = new QTimer;
    connect(castlehealth, SIGNAL(timeout()), this, SLOT(updatecastlehealth()));
    castlehealth->start(100);
    //adjsuting the castle's health bar
    QPixmap health(":/new/prefix1/fullhealth.jpg");
    health = health.scaledToWidth(100);
    health = health.scaledToHeight(100);
    castlehealthimage.setPixmap(health);
    castlehealthimage.setPos(50,10);
    scene->addItem(&castlehealthimage);

    //displaying a game timer so that the player know the remaining time

    QPixmap timerimage(":/new/prefix1/timer2.png");
    timerimage = timerimage.scaledToWidth(100);
    timerimage = timerimage.scaledToHeight(100);
    timeritem.setPixmap(timerimage);
    timeritem.setPos(850,0);
    scene->addItem(&timeritem);
    Focuschecker=new QTimer;
    connect(Focuschecker, SIGNAL(timeout()), this, SLOT(setFocusonDefense()));
    Focuschecker->start(50);
    elapsedtime = new QGraphicsTextItem;
    elapsedtime ->setFont(QFont("times",18));
    elapsedtime ->setDefaultTextColor(Qt::black);
    elapsedtime->setPlainText("0 : 0");
    elapsedtime->setPos(895,35);
    elapsedt=0;
    scene->addItem(elapsedtime);
    elapsedtimer=new QTimer;
    connect(elapsedtimer, SIGNAL(timeout()), this, SLOT(updateelapsedtime()));
    elapsedtimer->start(1000);
    //timer to generate the stronger enemy
    largerEnemyGenerator = new QTimer;
    largerEnemyGenerator->setSingleShot(true);
    connect(largerEnemyGenerator, SIGNAL(timeout()), this, SLOT(createLargerEnemy()));
    largerEnemyGenerator->start(18000);
    //calls the function that starts the game timer
    endthegame();
    //calls the earthquake function that works in levels with difficulty>3
    earthquake();

    //timer to generate healthmarkers that boost defense
    markerGenerator=new QTimer;
    connect(markerGenerator, SIGNAL(timeout()), this, SLOT(generateMarker()));
    markerGenerator->start(40000);//

    // Create a QPushButton
    QPushButton *pauseButton = new QPushButton();
    pauseButton->setFixedSize(60, 60); // Set the fixed size of the button
    pauseButton->setStyleSheet("border-radius: 20px; border: none;"); // Set circular shape and remove border
    pauseButton->setIcon(QIcon(":/new/prefix1/pause.png"));
    pauseButton->setIconSize(QSize(60, 60)); // Set icon size

    // Add the button to the scene
    QGraphicsProxyWidget *pauseProxy = scene->addWidget(pauseButton);
    pauseProxy->setPos(160, 10);

    // Connect the clicked signal of the button to a slot to pause the game
    connect(pauseButton, &QPushButton::pressed, this, &Level::pauseGame);
}
void Level::loadmap(const QString &location)
{
    QFile level1(location);
    QPixmap Cannon(":/new/prefix1/195_1.png");
    QPixmap troop(":/new/prefix1/838_0.png");
    QPixmap backgroundImage(":/new/prefix1/0_0.png");
    QGraphicsPixmapItem *backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setPos(-800,-500);
    scene->addItem(backgroundItem);
    //reading from the map file and adding items to the scene, nodes, and edges accordingly
    level1.open(QIODevice::ReadOnly);
    int r = 14;
    int c = 24;
    Board = new int* [r];
    for (int i = 0; i < r; i++)
    {
        Board[i] = new int[c];
    }
    QTextStream stream(&level1);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            QString character = stream.read(1);
            Board[i][j] = character.toInt();
            QString character2 = stream.read(1);
        }
    }
    int fences = 0;
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if(Board[i][j]==3)
                fences++;
        }
    }

    Fences = new Fence*[fences];
    citizens = new Citizen*[nofcitizens];
    int fencescreated = 0;
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if(Board[i][j] == 3)
            {
                Fences[fencescreated] = new Fence(scene,&view,i,j,c,r);
                gamegraph.addnode(i,j,5); //graph, fence nodes
                gamegraph.getnode(i,j)->setnodefence(Fences[fencescreated]);
                fencescreated++;

            }
            else if(Board[i][j] == 1)
            {
                gamecastle.setposition(j, i, scene);
                gamegraph.setgoal(i,j,0);//graph, castle node
                gamecastle.sethealth(100-((difficulty-1)*10)); //setting the health based on the difficulty
                for(int k =0; k<nofcitizens; k++)
                {
                    citizens[k] = new Citizen(scene, &view, 14, 24, Board);
                    int randomnum = QRandomGenerator::global()->bounded(1,200);


                    citizens[k]->setPos(gamecastle.getx()+randomnum, gamecastle.gety()+randomnum);
                }
            }
            else if(Board[i][j] == 2)
            {
                d1 = new Defence(i, j, scene, defensefile);
            }
            else if(Board[i][j]==0)
            {
                gamegraph.addnode(i,j,0); //graph, field nodes
            }
            else if(Board[i][j] == 4)
            {
                Item[i][j].setPixmap(troop);
                qreal scaleFactor = 1;
                Item[i][j].setScale(scaleFactor);
                Item[i][j].setPos((j * (1000/c)) , (i * (800/r)) + 50);
                scene->addItem(&Item[i][j]);
            }

        }
        nooffences=fencescreated;
    }


    //making edges
    for(int i =0; i<r; i++)
    {
        for(int j =0;j<c;j++)
        {
            if(gamegraph.doesnodeexist(i,j))
            {
                if(gamegraph.doesnodeexist(i+1, j))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i+1, j);
                    gamegraph.addedge(n1, n2, 1.67);
                }
                if(gamegraph.doesnodeexist(i-1, j))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i-1, j);
                    gamegraph.addedge(n1, n2, 1.67);
                }
                if(gamegraph.doesnodeexist(i, j+1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i, j+1);
                    gamegraph.addedge(n1, n2, 2.326);
                }
                if(gamegraph.doesnodeexist(i, j-1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i, j-1);
                    gamegraph.addedge(n1, n2, 2.326);
                }
                if(gamegraph.doesnodeexist(i+1, j+1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i+1, j+1);
                    gamegraph.addedge(n1, n2, 3.86);
                }
                if(gamegraph.doesnodeexist(i+1, j-1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i+1, j-1);
                    gamegraph.addedge(n1, n2, 3.86);
                }
                if(gamegraph.doesnodeexist(i-1, j+1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i-1, j+1);
                    gamegraph.addedge(n1, n2, 3.86);
                }
                if(gamegraph.doesnodeexist(i-1, j-1))
                {
                    node* n1 = gamegraph.getnode(i,j);
                    node* n2 = gamegraph.getnode(i-1, j-1);
                    gamegraph.addedge(n1, n2, 3.86);
                }
            }
        }
    }


    d1->setFlag(QGraphicsItem::ItemIsFocusable);
    d1->setFocus();
    view.show();
    QStringList imagePaths = {":/new/prefix1/1111_0.png", ":/new/prefix1/1112_0.png", ":/new/prefix1/1113_0.png"};


    int currentIndex = 0;
    //creating new enemies and adding them to the enemies list
    QObject::connect(&enemyTimer, &QTimer::timeout, [&]()
    {
        QPoint f(gamecastle.getx()+60,gamecastle.gety()+60);
        Enemy* enemy = new Enemy(f);
        enemies.append(enemy);
        enemy->setgraph(&gamegraph);
        enemy->getcurrentnode();
        enemy->setdestination();
        enemy->setspeed(sqrt(difficulty)*2.45); //setting the speed based on the difficulty, was 0.7
        enemy->sethealth(75+(25*difficulty));//setting the health based on the difficluty
        scene ->addItem(enemy);
    });
    enemyTimer.start(4000-((difficulty-1)*500)); //adjusting the timer based on the difficulty
}
void Level::addanitem(QGraphicsPixmapItem* a)
{
    scene -> addItem(a);
}
void Level::endthegame()
{

    int gametimer = 300000;
    leveltimer->setSingleShot(true); // this line makes the timer fires only once
    leveltimer->start(gametimer);
//checks if thhe game's time is out
    QObject::connect(leveltimer, &QTimer::timeout, [=]()
                     {
                         view.hide();
                         int n = Bullet::getkilled();
                         int g = Bullet::getgems();
                         pause();
                         //sending the game stats to the winning window to display the score
                         w->getscore(gamecastle.gethealth(),n,g);
                         //send the level's data for next levels based on the intials choices of the player
                         w->getleveldata(mapfile, defensefile, difficulty,AimAssist);
                         w->show();
                     });
}

void Level::addbackgroundmusic()
{
    QAudioOutput* mainthemeoutput = new QAudioOutput();
    mainthemeoutput ->setVolume(50);
    QMediaPlayer* maintheme = new QMediaPlayer();
    maintheme ->setAudioOutput(mainthemeoutput);
    maintheme->setSource(QUrl("qrc:/new/prefix1/longer one.mp3"));
    maintheme ->play();
}

QGraphicsScene* Level::getscene()
{
    return scene;
}
QGraphicsView* Level::getview()
{
    return &view;
}
void Level::createEnemy()
{
}
//adding broken fences to the queue
void Level::addBrokenFence(Fence *inFence)
{
    FencesDestroyed.push_back(inFence);
}
//checking if new fences are broken and assigning citizens to them accordingly
void Level::checkFences()
{
    for(int i =0; i<nooffences; i++)
    {
        if(Fences[i]->isbroken())
        {
            int icor = Fences[i]->geti();
            int jcor = Fences[i]->getj();
            node* a = gamegraph.getnode(icor, jcor);
            a->setweight(Fences[i]->weight);
            if (!FencesDestroyed.contains(Fences[i]))
            {
                //if no citizen is near to this fence then add it to the queue to be checked later
                if(!findNearestCitizen(Fences[i]))
                {
                 FencesDestroyed.push_back(Fences[i]);
                }

            }
        }
        else
        {
            int icor = Fences[i]->geti();
            int jcor = Fences[i]->getj();
            node* a = gamegraph.getnode(icor, jcor);
            a->setweight(Fences[i]->weight);
        }
    }
    if(!FencesDestroyed.empty())
    {
        for(int i =0; i<nofcitizens; i++)
        {
            if(!citizens[i]->isBusy()&&citizens[i]->isalive())
            {

                Fence* temp = citizens[i]->FindNearestFence(FencesDestroyed, gamecastle.getx(), gamecastle.gety(), d1->getx(), d1->gety());
                FencesDestroyed.erase(FencesDestroyed.begin()+citizens[i]->getfenceindex());
                temp->setfixed();
                citizens[i]->setmovetimer(temp->gety(),temp->getx(), temp);
                break;
            }
        }
    }

}
void Level::isgameover()
{
    if(gamecastle.gethealth()<0||gamecastle.gethealth()==0)
    {
        pause();
        int n = Bullet::getkilled();
        int g = Bullet::getgems();
        int p = Bullet::getusedpowerups();
        view.hide();
        GameOver* testgo = new GameOver;
        //send the stats of the game to the gameover window to displat them
        testgo->setstat(n,g, p);
        if(revived)
            testgo->setcont(); //checks if the user continued the game before and send this boolean to the gameover window to act accordingly
        testgo->setlevel(this);
        testgo->show();
    }

}
Castle* Level::getcastle()
{
    return &gamecastle;
}
void Level::setrevived()
{
    revived=true;
}
void Level::generategems()
{
    gem* gem1 = new gem();
    scene->addItem(gem1);
}

void Level::generatepowerup()
{
    int counter =0;
    //checks if all the citizens are dead. If so, generates the power up that revives them
    for(int i =0; i<nofcitizens;i++)
    {
        if(!citizens[i]->isalive())
            counter++;
    }
    if(counter == nofcitizens)
    {
        powerup* P = new powerup;
        scene->addItem(P);
    }
    QTimer* delaycheck = new QTimer;
    delaycheck->setSingleShot(true); // this line makes the timer fires only once
    delaycheck->start(10000);
    QObject::connect(delaycheck, &QTimer::timeout, [=]()
                     {
        //revives two citizens
        if(Bullet::getpowerups()>0)
                         {
                             Bullet::powerups=0;
                             citizens[0]->revive();
                             citizens[1]->revive();
                         }

                     }
                     );
}
void Level::updatecastlehealth()
{
    //adjust the health bar according to the castle's health
    if(gamecastle.gethealth()>69&&(gamecastle.gethealth()<100||gamecastle.gethealth()==100))
    {
        QPixmap health(":/new/prefix1/fullhealth.jpg");
        health = health.scaledToWidth(80);
        health = health.scaledToHeight(80);
        castlehealthimage.setPixmap(health);
    }
    else if(gamecastle.gethealth()<70&&gamecastle.gethealth()>40)
    {
        QPixmap health(":/new/prefix1/midhealth.jpg");
        health = health.scaledToWidth(80);
        health = health.scaledToHeight(80);
        castlehealthimage.setPixmap(health);
    }
    else if(gamecastle.gethealth()<40)
    {
        QPixmap health(":/new/prefix1/lowhealth.jpg");
        health = health.scaledToWidth(80);
        health = health.scaledToHeight(80);
        castlehealthimage.setPixmap(health);
    }
}
void Level::setFocusonDefense()
{
    d1->setFlag(QGraphicsItem::ItemIsFocusable);
    d1->setFocus();
}
//updates the game timer based on the elapsed time
void Level::updateelapsedtime()
{
    elapsedt++;
    int minutes = elapsedt / 60;
    int seconds = elapsedt % 60;
    QString m= QString::number(minutes);
    QString s= QString::number(seconds);
    elapsedtime->setPlainText(m +" : " +s);

}
int Level::getdifficulty()
{
    return difficulty;
}
//starts an earthquake if the difficulty is >3
void Level::earthquake()
{
    if(difficulty>3)
    {
        QPixmap backgroundImage(":/new/prefix1/11.png");
        QGraphicsPixmapItem *backgroundItem = new QGraphicsPixmapItem(backgroundImage);
        backgroundItem->setPos(0,200);
        backgroundItem->setZValue(0);
        scene->addItem(backgroundItem);

        QStringList imagePaths = {":/new/prefix1/11.png", ":/new/prefix1/12.png", ":/new/prefix1/13.png",":/new/prefix1/14.png",":/new/prefix1/15.png",":/new/prefix1/16.png",":/new/prefix1/17.png",":/new/prefix1/18.png",":/new/prefix1/19.png",":/new/prefix1/20.png",":/new/prefix1/21.png",":/new/prefix1/22.png",":/new/prefix1/23.png"};
        QTimer *timer = new QTimer(this);
        int currentIndex = 0;
        timer->setInterval(4000);
        connect(timer, &QTimer::timeout, [=]() mutable{
        if (currentIndex < imagePaths.size())
                {
                    QPixmap pixmap(imagePaths[currentIndex]);
                    backgroundItem->setPixmap(pixmap);
                    currentIndex++;
                    gamecastle.sethealth(gamecastle.gethealth()-2);
                    int randomindex = QRandomGenerator::global()->bounded(nooffences);
                    for(int i =0; i<5;i++)
                    {
                        Fences[randomindex]->decreasehealth();
                    }

                }
        else
                {
                    timer->stop();
                    disconnect(timer, &QTimer::timeout, nullptr, nullptr);
                    scene->removeItem(backgroundItem); //this line can be deleted later
                    timer->deleteLater();
                }
        });
        timer->start();
    }
}
QString Level::getmap()
{
    return mapfile;
}
QString Level::getdefence()
{
    return defensefile;
}
bool Level::findNearestCitizen(Fence *inFence)
{
    int min =1000;
    int mini;
    for(int i=0; i<nofcitizens; i++)
    {
        if((citizens[i]->isalive())&&!(citizens[i]->isBusy()))
        {
            float d = citizens[i]->FindDistanceFromFence(inFence,gamecastle.getx(), gamecastle.gety(), d1->getx(), d1->gety());
            if(d<min)
            {
                min = d;
                mini = i;
            }
        }
    }
    if(min!=1000)// so that if theyre all dead or busy, the function still works
    {
        inFence->setfixed();
        citizens[mini]->setmovetimer(inFence->gety(),inFence->getx(), inFence);
        return true;
    }
    return false;

}

// Generate health markers in the scene
void Level::generateMarker() {

    healthMarkers* h = new healthMarkers();
    scene->addItem(h);
}

void Level::createLargerEnemy() {
    if(difficulty>2) {
        QPoint f(gamecastle.getx()+60,gamecastle.gety()+60);
        largerEnemy* le = new largerEnemy(f);
        le->setdestination(f);
        scene->addItem(le);
    }
}


//a function that stops all of the game timers
void Level::pause()
{
    leveltimer->stop();
    enemyTimer.stop();
    markerGenerator->stop();
    fencechecker->stop();
    stopEnemies();
    gemsgenerator->stop();
    gameover->stop();
    elapsedtimer->stop();
    castlehealth->stop();
    largerEnemyGenerator->stop();
    citizentimer->stop();
    for(int i = 0; i < nofcitizens; i++) {
        citizens[i]->stop();
    }
}
//a function that starts all of the game timers

void Level::cont()
{

    leveltimer->start(300000-elapsedt);
    enemyTimer.start(4000-((difficulty-1)*500));
    fencechecker->start(200);
    largerEnemyGenerator->start();
    markerGenerator->start(20000);
    resumeEnemies();
    gemsgenerator->start(6000);
    gameover->start(100);
    castlehealth->start(100);
    elapsedtimer->start(1000);
    view.show();
    for(int i = 0; i < nofcitizens; i++) {
        citizens[i]->resume();
    }

    citizentimer->setInterval(200);

}

void Level::pauseGame() {

    pause();

    pw = new pauseWindow();
    pw->show();
//connecting the resume button of the pause window to the resume game function
    connect(pw, &pauseWindow::resumeGame, this, &Level::resumeGame);
    connect(pw, &pauseWindow::endGame, this, &Level::endGame);
}

void Level::resumeGame() {
    cont();
    pw->hide();
}

void Level::endGame() {
    view.hide();
}
//stops enemies' motion in case of pausing the games
void Level::stopEnemies() {
    for (Enemy* e : enemies) {
        e->stop();
    }
}
//resuming enemis' motion
void Level::resumeEnemies() {

    for (Enemy* e : enemies) {
        e->resume();
    }
}
//autoshooter that works based on the player's choice
//it shoots enemies near to the castle automatically
void Level::AutoShoot()
{
    for (Enemy* e : enemies) {
        if (e!= NULL && e->alive) {
            qreal distanceX =abs(gamecastle.x()-e->x());
            qreal distanceY =abs(gamecastle.y()-e->y());
            if (distanceX<200 && distanceY<150) {

                if(e->current=="right")
                {
                    d1->shoot(e->x()+20,e->y());

                    e->shot = true;
                }
                if(e->current=="left")
                {
                    d1->shoot(e->x()-20,e->y());

                    e->shot = true;
                }
                if(e->current=="down")
                {
                    d1->shoot(e->x(),e->y()+10);

                    e->shot = true;
                }
                if(e->current=="up")
                {
                    d1->shoot(e->x(),e->y()-10);
                    e->shot = true;
                }
            }
        }
    }
}
