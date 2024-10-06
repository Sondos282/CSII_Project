#include "settings.h"
#include "ui_settings.h"
#include "menu.h"
#include "ui_menu.h"
#include<QFile>
#include<QTextStream>
#include <QApplication>
#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QString>
#include<iostream>
#include"citizen.h"
#include"fence.h"
#include<QTimer>
#include "level.h"
#include<QDebug>
settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings)
{
    this->parent = parent;
    ui->setupUi(this);
    QPixmap backpix (":/new/prefix1/background image (1).jpg");
    int w = ui -> label -> width();
    int h = ui -> label -> height();
    backpix = backpix.scaledToWidth(w);
    backpix = backpix.scaledToHeight(h);
    ui -> label->setPixmap(backpix.scaled(w+10, h+10, Qt::KeepAspectRatio));
    ui->label_error->setVisible(false);
    ui->label_error2->setVisible(false);
    ui->label_error_4->setVisible(false);
    ui->label_error2_2->setVisible(false);
    ui->Cannon->setChecked(true);
    ui->radioButton->setChecked(true);
    view.setFixedSize(1000, 800);
    view.setWindowTitle("Clash of Clans");
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,1000,800);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFixedSize(400,400);


    view.setScene(scene);
}

settings::~settings()
{
    delete ui;
}
void settings::setbuttons(QString a, QString b, bool c, bool d)
{
    QString location;
    if(a==":/new/prefix1/LevelEx.txt")
    {
        location = a;
        ui->radioButton->setChecked(true);
    }
    else
    {
        location = ":/new/prefix1/LevelEx_2.txt";
        ui->radioButton_2->setChecked(true);
    }
    if(b == ":/new/prefix1/1675_0.png")
    {
        ui->Archer->setChecked(true);
    }
    else if(b==":/new/prefix1/877_0.png")
    {
        ui->Wizzard->setChecked(true);
    }
    else
        ui->Cannon->setChecked(true);
    if(c==true)
    {
        ui->radioButton_3->setChecked(true);
    }
    else
        ui->radioButton_4->setChecked(true);
    if(d==false)
    {
        ui->radioButton_6->setChecked(true);
    }
    else
        ui->radioButton_5->setChecked(true);



    previewMap(location, ui->graphicsView->scene());
    ui->graphicsView->show();
}
void settings::on_pushButton_clicked()
{
    this->hide();
    Menu* m=new Menu;
    bool music;
    QString map;
    QString defence;
    bool AimAssist;

    if(b3())
    {
        music = true;
    }
    else
        music = false;
    if(b1())
    {
        map = ":/new/prefix1/LevelEx.txt";
    }
    else
        map = ":/new/prefix1/LevelEx_2.txt";
    if(archer())
    {
        defence = ":/new/prefix1/1675_0.png";
    }
    else if(wizard())
    {
        defence = ":/new/prefix1/877_0.png";
    }
    else
    {
        defence = ":/new/prefix1/195_1.png";
    }
    if(ui->radioButton_5->isChecked())
    {
        AimAssist = true;
    }
    else
        AimAssist = false;
    m->setmapanddefence(map, defence, music,AimAssist);
    m->show();
}


void settings::on_pushButton_2_clicked()
{
    ui->label_error_4->setVisible(false);
    bool b1 = ui->radioButton->isChecked();
    bool b2 = ui->radioButton_2->isChecked();
    if(b1)
    {
        QString location = ":/new/prefix1/LevelEx.txt";
        previewMap(location, ui->graphicsView->scene());
        ui->graphicsView->show();
    }
    if(b2)
    {
        QString location = ":/new/prefix1/LevelEx_2.txt";
        previewMap(location, ui->graphicsView->scene());
        ui->graphicsView->show();

    }
}
bool settings::b1()
{
    return(ui->radioButton->isChecked());
}
bool settings::b2()
{
    return(ui->radioButton_2->isChecked());
}
bool settings::b3()
{
    return(ui->radioButton_3->isChecked());
}
bool settings::b4()
{
    return(ui->radioButton_4->isChecked());
}
bool settings::wizard()
{
    return(ui->Wizzard->isChecked());
}
bool settings::cannon()
{
    return(ui->Cannon->isChecked());
}
bool settings::archer()
{
    return(ui->Archer->isChecked());
}
void settings::previewMap(const QString file,QGraphicsScene *inscene)
{
    bool cannon = ui->Cannon->isChecked();
    bool wizard = ui->Wizzard->isChecked();
    bool archer = ui->Archer->isChecked();
    if((!cannon) && (!wizard) && (!archer))
    {
        ui->label_error_4->show();
        return;
    }

    QFile level1(file);
    level1.open(QIODevice::ReadOnly);

    int r = 14;
    int c = 24;
    int Board[r][c];
    QTextStream stream(&level1);

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            QString character = stream.read(1);
            Board[i][j] = character.toInt();
            QString character2 = stream.read(1);
        }
    }
    QPixmap fence(":/new/prefix1/fw.png");
    QPixmap Cannon(":/new/prefix1/195_1.png");
    QPixmap WizardTower(":/new/prefix1/877_0.png");
    QPixmap ArcherTower(":/new/prefix1/1675_0.png");
    QPixmap Castle(":/new/prefix1/town.png");


    QPixmap backgroundImage(":/new/prefix1/0_0.png");


    qreal bgX = (inscene->width() - backgroundImage.width()) / 2.0;
    qreal bgY = (inscene->height() - backgroundImage.height()) / 2.0;

    QGraphicsPixmapItem *backgroundItem = new QGraphicsPixmapItem(backgroundImage);
    backgroundItem->setPos(bgX, bgY);

    inscene->addItem(backgroundItem);



    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            QGraphicsPixmapItem *item = new QGraphicsPixmapItem();
            if(Board[i][j] == 3)
            {
                qreal scaleFactor = 0.25;
                qreal x = 290+(j * (400/c));
                qreal y = 220+ (i * (400/r));
                fence=fence.scaled(30,30);
                item->setPixmap(fence);
                //item->setScale(scaleFactor);
                item->setPos(x,y);
                inscene->addItem(item);
            }
            else if(Board[i][j] == 1)
            {
                qreal scaleFactor = 0.1;
                qreal x = 270+(j * (400/c));
                qreal y = 200+ (i * (400/r));
                Castle=Castle.scaled(65,65);
                item->setPixmap(Castle);
                item->setPos(x,y);
                inscene->addItem(item);
            }
            else if(Board[i][j] == 2)
            {
                qreal x = 290+(j * (400/c));
                qreal y = 220+ (i * (400/r));

                if(cannon)
                {
                    qreal scaleFactor = 0.3;
                    item->setPixmap(Cannon);
                    item->setScale(scaleFactor);
                    item->setPos(x,y);
                    inscene->addItem(item);
                }
                else if(wizard)
                {
                    qreal scaleFactor = 0.2;
                    item->setPixmap(WizardTower);
                    item->setScale(scaleFactor);
                    item->setPos(x,y);
                    inscene->addItem(item);
                }
                else if(archer)
                {
                    qreal scaleFactor = 0.2;
                    item->setPixmap(ArcherTower);
                    item->setScale(scaleFactor);
                    item->setPos(x,y);
                    inscene->addItem(item);
                }
            }
        }
    }
}

