
#include "fence.h"
#include<QTimer>

Fence::Fence(QGraphicsScene *inscene, QGraphicsView *inview, int i, int j, int c, int r): QObject() {

    health = 100; // Initializing the health of the fence

    weight = 5; // Initializing the weight of the fence

    QPixmap fence(":/new/prefix1/45_0.png"); // Loading the pixmap for the fence
    qreal scaleFactor = 0.5; // Setting the scale factor for the fence
    this->setScale(scaleFactor); // Scaling the fence
    fence = fence.scaledToWidth(50); // Scaling the fence pixmap to width
    fence = fence.scaledToHeight(50); // Scaling the fence pixmap to height
    setPixmap(fence); // Setting the pixmap for the fence

    this->i = i; // Setting the row index of the fence
    this->j = j; // Setting the column index of the fence
    this->setPos((j * (1000/c))+7 , (i * (800/r)) + 50); // Setting the position of the fence on the scene

    scene = inscene; // Assigning the scene
    view = inview; // Assigning the view
    scene->addItem(this); // Adding the fence to the scene

    QTimer * timer = new QTimer(this); // Creating a QTimer object for fence design updates
    connect(timer, SIGNAL(timeout()), this, SLOT(fenceDesign())); // Connecting the timer signal to the fenceDesign slot
    timer->start(100); // Starting the timer with a timeout of 100 milliseconds
    broken = false; // Setting the broken boolean to false initially
}

// Function to get the row index of the fence
int Fence::geti(){
    return i;
}

// Function to get the y-coordinate of the fence
int Fence::gety(){
    return (j * (1000/24));
}

// Function to get the x-coordinate of the fence
int Fence::getx(){
    return ((i * (800/14)) +50);
}

// Function to get the column index of the fence
int Fence::getj(){
    return j;
}

// Function to check if the fence is at full health
bool Fence::fullHealth(){
    return health==100;
}

// Function to increment the health of the fence
void Fence::incrementHealth()
{
    health += 25; // Increasing the health by 25
}

// Function to check if the fence is broken
bool Fence::isbroken()
{
    return broken; // Returning the broken boolean
}

// Function to set the health of the fence
void Fence::sethealth(int h)
{
    health = h; // Setting the health to the given value
    broken = true; // Setting the broken boolean to true
    weight = 0; // Setting the weight to 0
}

// Function to set the fence as fixed
void Fence::setfixed()
{
    broken = false; // Setting the broken boolean to false
}

// Function to decrease the health of the fence
void Fence::decreasehealth()
{
    if(health > 0)
        health -= 4; // Decreasing the health by 4
    if(health == 0 || health < 0)
    {
        broken = true; // Setting the broken boolean to true if health is 0 or less
        weight = 0; // Setting the weight to 0
    }
}

// Function to update the fence design based on its health
void Fence::fenceDesign()
{
    QPixmap full(":/new/prefix1/fw.png"), halfbroken (":/new/prefix1/brokenfence.png"), broken( ":/new/prefix1/493_0.png"); // Loading pixmaps for different fence states

    full = full.scaledToWidth(90); // Scaling the full health fence pixmap to width
    full = full.scaledToHeight(90); // Scaling the full health fence pixmap to height

    halfbroken = halfbroken.scaledToHeight(90); // Scaling the half broken fence pixmap to height
    halfbroken = halfbroken.scaledToWidth(90); // Scaling the half broken fence pixmap to width

    if(health > 50 || health == 50) // Checking if the fence is at full health or above
    {
        setPixmap(full); // Setting the pixmap to full health fence
        weight = 5; // Setting the weight to 5
    }
    else if(health > 0) // Checking if the fence is partially broken
    {
        setPixmap(halfbroken); // Setting the pixmap to half broken fence
        weight = 3; // Setting the weight to 3
    }
    else if(health == 0) // Checking if the fence is completely broken
    {
        setPixmap(broken); // Setting the pixmap to broken fence
        weight = 0; // Setting the weight to 0
    }
}
