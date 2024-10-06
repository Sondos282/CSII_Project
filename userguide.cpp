
#include "userguide.h"
#include "ui_userguide.h"
#include"menu.h"

userguide::userguide(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::userguide)
{
    ui->setupUi(this);

    QPixmap pix (":/new/prefix1/userguidebg.png"); // Creating a QPixmap object with the user guide background image
    int w = ui -> label -> width(); // Getting the width of the label in the UI
    int h = ui -> label -> height(); // Getting the height of the label in the UI
    ui -> label ->setPixmap(pix); // Setting the pixmap of the label to display the user guide background image

    ui-> pushButton ->setStyleSheet("background-color: transparent; border: none;"); // Styling the push button to make it transparent with no border
}

void userguide::set(QString ma, QString d, bool m,bool a)
{
    map =ma; // Setting the map file
    def =d; // Setting the defense structure
    music = m; // Setting the background music
    AimAssist=a; // Setting the AimAssist
}

userguide::~userguide()
{
    delete ui;
}

void userguide::on_pushButton_clicked()
{
    this->hide(); // Hiding the user guide dialog
    Menu* m = new Menu; // Creating a new Menu object
    m->setmapanddefence(map, def, music,AimAssist); // Setting the map, defense, music, and AimAssist attributes for the Menu object
    m->show(); // Displaying the Menu object
    delete this; // Deleting the user guide dialog
}
