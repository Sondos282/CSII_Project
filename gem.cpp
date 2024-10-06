// #include "gem.h"
// #include<QTimer>
// #include<QRandomGenerator>
// gem::gem()
// {
//     QPixmap gem(":/new/prefix1/gemm.png");
//     gem = gem.scaledToWidth(30);
//     gem = gem.scaledToHeight(30);
//     setPixmap(gem);
//     QTimer * hidegem = new QTimer(this);
//     hidegem->setSingleShot(true);
//     hidegem->start(3000);
//     int xpos = QRandomGenerator::global()->bounded(50, 950);
//     int ypos = QRandomGenerator::global()->bounded(50, 750);
//     setPos(xpos, ypos);
//     QObject::connect(hidegem, &QTimer::timeout, [=]()
//                      {
//                          delete this;
//                      });
// }


#include "gem.h"
#include<QTimer>
#include<QRandomGenerator>

gem::gem()
{
    QPixmap gem(":/new/prefix1/gemm.png"); // Loading the pixmap for the gem image
    gem = gem.scaledToWidth(30); // Scaling the pixmap to a specific width
    gem = gem.scaledToHeight(30); // Scaling the pixmap to a specific height
    setPixmap(gem); // Setting the pixmap for the gem item

    QTimer * hidegem = new QTimer(this); // Creating a QTimer object to hide the gem
    hidegem->setSingleShot(true); // Setting the timer to fire only once
    hidegem->start(3000); // Starting the timer with a timeout of 3000 milliseconds (3 seconds)

    int xpos = QRandomGenerator::global()->bounded(50, 950); // Generating a random x position for the gem
    int ypos = QRandomGenerator::global()->bounded(50, 750); // Generating a random y position for the gem
    setPos(xpos, ypos); // Setting the position of the gem item

    // Connecting the timeout signal of the timer to a lambda function to delete the gem item when the timer expires
    QObject::connect(hidegem, &QTimer::timeout, [=]()
                     {
                         delete this; // Deleting the gem item
                     });
}
