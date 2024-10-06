#include "healthmarkers.h"
#include <QTimer>
#include "QRandomGenerator"

healthMarkers::healthMarkers() {

    // Set the appearance of the health marker
    QPixmap pix(":/new/prefix1/health_booster-removebg-preview.png");
    pix = pix.scaledToWidth(40);
    pix = pix.scaledToHeight(40);
    setPixmap(pix);

    // Create a timer to hide the health marker after a certain duration
    QTimer* hideBooster = new QTimer(this);
    hideBooster->setSingleShot(true); // Set the timer to fire only once
    hideBooster->start(10000); // Hide the health marker after 10000 milliseconds (10 seconds)

    // Generate random positions for the health markers within the scene bounds
    int xpos = QRandomGenerator::global()->bounded(50, 950); // Random x-coordinate
    int ypos = QRandomGenerator::global()->bounded(50, 750); // Random y-coordinate
    setPos(xpos, ypos); // Set the position of the powerup

    // Connect the timer's timeout signal to delete the health marker object
    QObject::connect(hideBooster, &QTimer::timeout, [=]() {
        delete this; // Delete the health marker object when the timer times out
    });
}
