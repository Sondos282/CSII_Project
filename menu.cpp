#include <QApplication>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QPixmap>
#include<QGraphicsPixmapItem>
#include<QFile>
#include"level.h"
#include<QTextStream>
#include<QCursor>
#include<QVector2D>
#include"defense.h"
#include"menu.h"
#include "ui_menu.h"
#include"userguide.h"
Menu::Menu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);
    QPixmap pix (":/new/prefix1/background image (1).jpg");
    int w = ui -> label_image -> width();
    int h = ui -> label_image -> height();
    pix = pix.scaledToWidth(w);
    pix = pix.scaledToHeight(h);
    ui -> label_image ->setPixmap(pix.scaled(w+10, h+10, Qt::KeepAspectRatio));
    ui-> gamemanual ->setStyleSheet("background-color: transparent; border: none;");
    QPixmap pixmap(":/new/prefix1/info.png");
    int w1 = ui -> info -> width();
    int h1 = ui -> info -> height();
    ui -> info ->setPixmap(pixmap.scaled(w1+10, h1+10, Qt::KeepAspectRatio));
    map = ":/new/prefix1/LevelEx.txt";
    defence = ":/new/prefix1/195_1.png";
    music = false;
    AimAssist = false;

}

Menu::~Menu()
{
    delete ui;

}

void Menu::on_gamemanual_clicked()
{
    this->hide();
    userguide* userGuide = new userguide;
    userGuide->set(map, defence, music, AimAssist);
    userGuide->show();
}

void Menu::setmapanddefence(QString m, QString d, bool music, bool inAim)
{
    map =m;
    defence=d;
    this->music = music;
    AimAssist=inAim;
}

void Menu::on_pushButton_clicked()
{
    this->hide();
    Level *l1 = new Level(map, defence, 1,AimAssist);
    if(music)
        l1->addbackgroundmusic();
}


void Menu::on_pushButton_2_clicked()
{
    this->hide();
    settings* s = new settings();
    s->setbuttons(map, defence, music,AimAssist);
    s->show();
}
