
#include "castle.h"

Castle::Castle() {
}

void Castle::setposition(int x, int y, QGraphicsScene* s)
{
    // Set the position of the castle on the scene
    xpos = (1000 / 24) * x;
    ypos = (800 / 14) * y;

    health = 100; // Initialize castle health

    QPixmap castle(":/new/prefix1/town.png"); // Load castle image
    setPixmap(castle.scaled(120, 120)); // Scale castle image
    this->setPos(xpos, ypos); // Set position of the castle

    scene = s; // Set the scene
    s->addItem(this); // Add the castle to the scene
}

int Castle::getx()
{
    return xpos; // Return x position of the castle
}

int Castle::gety()
{
    return ypos; // Return y position of the castle
}

void Castle::decreasehealth()
{
    health = health - 0.5; // Decrease castle health
}

qreal Castle::gethealth()
{
    return health; // Return current health of the castle
}

void Castle::sethealth(int x)
{
    health = x; // Set the health of the castle
}

bool Castle::checkhealth()
{
    if (health == 0 || health < 0)
    {
        return true; // If castle health is damaged
    }
    else
        return false; // If castle is not damaged
}
