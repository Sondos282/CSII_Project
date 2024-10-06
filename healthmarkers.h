#ifndef HEALTHMARKERS_H
#define HEALTHMARKERS_H

#include <QGraphicsPixmapItem>

class healthMarkers: public QObject, public QGraphicsPixmapItem
{
public:
    healthMarkers();

signals:
    void markerCollected(); // Signal emitted when a health marker is collected
};

#endif // HEALTHMARKERS_H
