#include "pausewindow.h"
#include "ui_pausewindow.h"
#include "menu.h"

pauseWindow::pauseWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pauseWindow)
{
    ui->setupUi(this);

    setWindowTitle("Pause");
    setFixedSize(400, 300);
    QPixmap pix(":/new/prefix1/pauseBack.jpg");
    pix = pix.scaledToHeight(400);
    pix = pix.scaledToWidth(400);
    ui->label_back->setPixmap(pix);
    ui-> pushButton_resume ->setStyleSheet("background-color: transparent; border: none;");
    ui-> pushButton_exit->setStyleSheet("background-color: transparent; border: none;");

}

pauseWindow::~pauseWindow()
{
    delete ui;
}

void pauseWindow::on_pushButton_resume_clicked()
{
    emit resumeGame();
}

void pauseWindow::on_pushButton_exit_clicked()
{
    hide();
    Menu* m = new Menu;
    m->show();
    emit endGame();
}

