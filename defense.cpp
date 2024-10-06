
#include "defense.h"
#include<QMouseEvent>
#include<QCursor>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsScene>
#include"bullet.h"
#include<QTimer>

Defence::Defence(int y, int x, QGraphicsScene* s, QString infile)
{
    //setting the position of the defense
    xpos = (1000/24)*x;
    ypos = (800/14)*y;

    strength = 1; // Initializing the strength of the defense

    powerBoost=false; // Setting the power boost to false initially

    QPixmap defenceunit(infile); // Loading the pixmap for the defense unit from the provided file path
    setPixmap(defenceunit.scaled(100,100)); // Scaling the pixmap to fit the defense unit
    scene = s; // Assigning the scene
    this->setPos(xpos, ypos); // Setting the position of the defense unit on the scene
    s->addItem(this); // Adding the defense unit to the scene
}

// Function to shoot a bullet from the defense unit
void Defence::shoot(int x, int y)
{
    QPoint a(x,y); // Creating a QPoint with the given coordinates
    Bullet* bullet = new Bullet(true,a); // Creating a bullet object with auto-shoot enabled
    bullet->setPos(xpos+50, ypos-10); // Setting the position of the bullet relative to the defense unit
    bullet->setRotation(40); // Setting the rotation angle of the bullet
    scene->addItem(bullet); // Adding the bullet to the scene
}

// Function to handle key press events for the defense unit
void Defence::keyPressEvent(QKeyEvent *event)
{
    if(event->key()== Qt::Key_Space)
    {
        if(!powerBoost)
        {
            // If power boost is not activated
            Bullet* bullet = new Bullet(false, QCursor::pos(),false); // Creating a bullet object with manual shoot
            bullet->setdefenseunit(this); // Setting the defense unit as the owner of the bullet
            bullet->setPos(xpos+50, ypos-10); // Setting the position of the bullet relative to the defense unit
            bullet->setRotation(40); // Setting the rotation angle of the bullet
            scene->addItem(bullet); // Adding the bullet to the scene
        }
        else
        {
            // If power boost is activated
            Bullet* bullet = new Bullet(false, QCursor::pos(), true); // Creating a bullet object with manual shoot and power boost
            bullet->setdefenseunit(this); // Setting the defense unit as the owner of the bullet
            bullet->setPos(xpos+50, ypos-10); // Setting the position of the bullet relative to the defense unit
            bullet->setRotation(40); // Setting the rotation angle of the bullet
            scene->addItem(bullet); // Adding the bullet to the scene
        }
    }
}

// Function to get the x-coordinate of the defense unit
int Defence::getx()
{
    return xpos; // Returning the x-coordinate
}

// Function to get the y-coordinate of the defense unit
int Defence::gety()
{
    return ypos; // Returning the y-coordinate
}

// Function to activate power boost
void Defence::setpowerBoosted()
{
    powerBoost=true; // Setting power boost to true
    QTimer::singleShot(30000, [this]()
                       {
                           powerBoost=false; // Deactivating power boost after 30 seconds
                       });
}
