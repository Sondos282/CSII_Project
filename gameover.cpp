#include "gameover.h"
#include "ui_gameover.h"
GameOver::GameOver(QWidget *parent, QGraphicsView* v)
    : QDialog(parent)
    , ui(new Ui::GameOver)
{
    gview=v;
    ui->setupUi(this);
    cont = false;
    QPixmap pix (":/new/prefix1/gameover2.jpg");
    int w = ui -> label -> width();
    int h = ui -> label -> height();
    pix = pix.scaledToWidth(w);
    pix = pix.scaledToHeight(h);
    ui -> label ->setPixmap(pix.scaled(w+10, h+10, Qt::KeepAspectRatio));
    ui-> continue_2 ->setStyleSheet("background-color: transparent; border: none;");
    ui-> exit ->setStyleSheet("background-color: transparent; border: none;");
    QFont font = ui->exit->font();
    font.setPointSize(16);
    ui->exit->setFont(font);
    QFont font2 = ui->continue_2->font();
    font2.setPointSize(16);
    ui->continue_2->setFont(font2);
    ui->error->setVisible(false);
}

GameOver::~GameOver()
{
    delete ui;
}
void GameOver::setstat(int x, int g, int p)
{
    QString xstr = QString::number(x);
    QString gstr = QString::number(g);
    QString pstr = QString::number(p);
    gems = g;
    QColor textcolor(217,216,215);
    ui->killed->setStyleSheet("color: "+textcolor.name());
    QFont font2 = ui->killed->font();
    font2.setPointSize(22);
    ui->killed->setFont(font2);
    ui->killed->setText(xstr);
    ui->gems->setStyleSheet("color: "+textcolor.name());
    ui->gems->setFont(font2);
    ui->gems->setText(gstr);
    ui->powerups->setStyleSheet("color: "+textcolor.name());
    ui->powerups->setFont(font2);
    ui->powerups->setText(pstr);

}
void GameOver::on_exit_clicked()
{
    close();
}

bool GameOver::getcont()
{
    return cont;
}
void GameOver::setcont()
{
    cont = true;
}
void GameOver::on_continue_2_clicked()
{
    //ensures that at least three gems were collected before the user can continue the game
    if((gems>3||gems==3)&&!cont)
    {
        cont = true;
        this->hide();
        (inlevel->getcastle())->sethealth(100);
        inlevel->setrevived(); //this is used to make sure that if the player continued and lost the game, the player won't be able to continue again
        inlevel->cont();
    }
    else
    {
        ui->error->setVisible(true);
    }

}
void GameOver::setview(QGraphicsView* v)
{gview = v;}
void GameOver::setlevel(Level* l1)
{inlevel =l1;}
