#include "powerup.h"
#include<QTimer>
#include<QRandomGenerator>

powerup::powerup() {

    QPixmap powerup(":/new/prefix1/elixir.png"); // Creating a QPixmap object with the powerup image
    powerup= powerup.scaledToWidth(40); // Scaling the powerup image to a specific width
    powerup = powerup.scaledToHeight(40); // Scaling the powerup image to a specific height
    setPixmap(powerup); // Setting the pixmap of the powerup item

    QTimer * hidepowerup = new QTimer(this); // Creating a QTimer object for hiding the powerup item
    hidepowerup->setSingleShot(true); // Setting the timer to fire only once
    hidepowerup->start(10000); // Starting the timer with a timeout of 10 seconds

    int xpos = QRandomGenerator::global()->bounded(50, 950); // Generating a random x position within a range
    int ypos = QRandomGenerator::global()->bounded(50, 750); // Generating a random y position within a range
    setPos(xpos, ypos); // Setting the position of the powerup item

    QObject::connect(hidepowerup, &QTimer::timeout, [=]() // Connecting the timeout signal of the timer to a lambda function
                     {
                         delete this; // Deleting the powerup item when the timer times out
                     });
}
