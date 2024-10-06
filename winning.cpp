#include "winning.h"
#include "ui_winning.h"
#include "level.h"

winning::winning(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::winning)
{
    ui->setupUi(this);

    QPixmap pix (":/new/prefix1/new winning.jpg"); // Creating a QPixmap object with the winning image
    int w = ui -> label -> width(); // Getting the width of the label in the UI
    int h = ui -> label -> height(); // Getting the height of the label in the UI
    pix = pix.scaledToWidth(w); // Scaling the pixmap to fit the width of the label
    pix = pix.scaledToHeight(h); // Scaling the pixmap to fit the height of the label
    ui -> label ->setPixmap(pix.scaled(w+10, h+10, Qt::KeepAspectRatio)); // Setting the pixmap of the label to display the winning image

    ui-> pushButton ->setStyleSheet("background-color: transparent; border: none;"); // Styling the push button to make it transparent with no border
    ui-> pushButton_2 ->setStyleSheet("background-color: transparent; border: none;"); // Styling the push button to make it transparent with no border

    ui->error->setVisible(false); // Hiding the error message initially
}

void winning::getscore(int x, int k, int g)
{
    int score = x*270+k*40+g*21; // Calculating the total score based on the number of enemies killed, gems collected, and power-ups obtained
    QString xstr = QString::number(score); // Converting the score to a string
    ui->score->setText(xstr); // Setting the score text in the UI

    QFont font = ui->score->font(); // Getting the font of the score text
    font.setPointSize(24); // Setting the font size
    ui->score->setFont(font); // Applying the font to the score text

    QColor text(217,216,216); // Creating a color for the text
    ui->score->setStyleSheet("color: "+text.name()); // Setting the color of the score text
    ui->score->setAlignment(Qt::AlignCenter); // Aligning the score text to the center
}

winning::~winning()
{
    delete ui;
}

void winning::on_pushButton_clicked()
{

    if(difficulty>4) // Checking if the difficulty level is greater than 4
    {
        ui->error->setVisible(true ); // Displaying the error message
        qDebug()<<"here is the 1sttttt problem"; // Outputting debug information
    }
    else
    {
        this->hide(); // Hiding the winning dialog
        qDebug()<<"here is the 2nd problem"; // Outputting debug information
        if(difficulty+1==2)
        {
            map =":/new/prefix1/level2.txt"; // Setting the map file path for the next level
        }
        else if(difficulty+1==3)
        {
            map =":/new/prefix1/level3.txt"; // Setting the map file path for the next level
        }
        else if(difficulty+1==4)
        {
            map =":/new/prefix1/level4.txt"; // Setting the map file path for the next level
        }
        else if(difficulty+1==5)
        {
            map =":/new/prefix1/level5.txt"; // Setting the map file path for the next level
        }
        Level* newlevel=new Level(map, defence, difficulty+1,AimAssist); // Creating a new Level object for the next level
        newlevel->show(); // Displaying the next level
    }
}

void winning::getleveldata(QString m, QString d, int dif, bool aim)
{
    map=m; // Setting the map file path
    defence = d; // Setting the defense image path
    difficulty=dif; // Setting the difficulty level
    AimAssist=aim; // Setting the AimAssist option
}

void winning::on_pushButton_2_clicked()
{
    close(); // Closing the winning dialog
    this->hide(); // Hiding the winning dialog
    delete this; // Deleting the winning dialog
}
