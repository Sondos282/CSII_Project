#include "bullet.h"
#include <QPixmap>
#include<QTimer>
#include<QCursor>
#include"enemy.h"
#include<qmath.h>
#include"powerup.h"
#include"gem.h"
#include"healthmarkers.h"

Bullet::Bullet(bool autoshoot, QPoint fp, bool power)
{
    // Load bullet image
    QPixmap bullet(":/new/prefix1/firee.png");

    // Scale bullet image
    bullet = bullet.scaledToWidth(40);
    bullet = bullet.scaledToHeight(40);

    // Set bullet image
    setPixmap(bullet);

    // Set power boost
    powerBoosted = power;

    // Adjust starting position
    int xp = fp.x() - 15;
    int yp = fp.y() - 45;
    QPoint z(xp, yp);
    if (autoshoot)
    {
        finalpoint = fp;
    }
    else
    {
        finalpoint = z;
    }

    // Calculate distances for target point
    distancex = abs(450 - finalpoint.x());
    distancey = abs(200 - finalpoint.y());

    // Create a timer for bullet movement
    QTimer * movetimer = new QTimer(this);
    connect(movetimer, SIGNAL(timeout()), this, SLOT(move()));
    movetimer->start(50); // Start the timer
}

int Bullet::killed = 0; // Initialize killed counter
int Bullet::gems = 0; // Initialize gems counter
int Bullet::powerups = 0; // Initialize powerups counter
int Bullet::usedpowerups = 0; // Initialize used powerups counter

void Bullet::move()
{
    // Calculate direction vector towards final point
    QPointF direction = finalpoint - pos();

    // Calculate the length of the direction vector
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    // Normalize the direction vector if its length is greater than zero
    if (length > 0) {
        direction /= length;
    }

    // Set bullet speed
    qreal speed = 15;

    // Move the bullet towards the final point
    setPos(x() + speed * direction.x(), y() + speed * direction.y());

    // Check if the bullet reached the final point
    qreal distanceToFinalPoint = QLineF(pos(), finalpoint).length();
    if (distanceToFinalPoint <= speed) {
        setPos(finalpoint);
        delete this; // Remove the bullet from the scene
        return;
    }

    // Check for collisions with other items
    QList<QGraphicsItem*> colliding_items = collidingItems();
    int n = colliding_items.size();
    for(int i = 0; i < n; i++)
    {
        // Check if the colliding item is an enemy
        if(Enemy* enemy = dynamic_cast<Enemy*>(colliding_items[i])) {
            if(powerBoosted) {
                enemy->alive = false; // Mark enemy as dead
                scene()->removeItem(enemy); // Remove enemy from the scene
                killed++; // Increment killed counter
            }
            else if(killed % 20) {
                enemy->decreaseHealth(50 + (50 * 0.1)); // Decrease enemy health
                if(enemy->getHealth() == 0 || enemy->getHealth() < 0)
                {
                    enemy->alive = false; // Mark enemy as dead
                    scene()->removeItem(enemy); // Remove enemy from the scene
                    killed++; // Increment killed counter
                }
            }
            else {
                enemy->decreaseHealth(50); // Decrease enemy health
                if(enemy->getHealth() == 0 || enemy->getHealth() < 0)
                {
                    enemy->alive = false; // Mark enemy as dead
                    scene()->removeItem(enemy); // Remove enemy from the scene
                    killed++; // Increment killed counter
                }
            }
            scene()->removeItem(this); // Remove the bullet from the scene
            delete this; // Delete the bullet object
            return;
        }

        // Check if the colliding item is a powerup
        if(typeid(*(colliding_items[i])) == typeid(powerup))
        {
            scene()->removeItem(colliding_items[i]); // Remove the powerup from the scene
            scene()->removeItem(this); // Remove the bullet from the scene
            powerups++; // Increment powerups counter
            usedpowerups++; // Increment used powerups counter
            delete colliding_items[i]; // Delete the powerup object
            delete this; // Delete the bullet object
            return;
        }

        // Check if the colliding item is a gem
        if(typeid(*(colliding_items[i])) == typeid(gem))
        {
            scene()->removeItem(colliding_items[i]); // Remove the gem from the scene
            scene()->removeItem(this); // Remove the bullet from the scene
            gems++; // Increment gems counter
            delete colliding_items[i]; // Delete the gem object
            delete this; // Delete the bullet object
            return;
        }

        // Check if the colliding item is a health marker
        else if(typeid(*(colliding_items[i])) == typeid(healthMarkers))
        {
            DUnit->setpowerBoosted(); // Activate power boost
            scene()->removeItem(colliding_items[i]); // Remove the health marker from the scene
            scene()->removeItem(this); // Remove the bullet from the scene
            delete colliding_items[i]; // Delete the health marker object
            delete this; // Delete the bullet object
            return;
        }
    }
}

void Bullet::setdefenseunit(Defence* d)
{
    DUnit = d; // Set the defense unit
}

void Bullet::activatePowerBoost()
{
    powerBoosted = true; // Activate power boost
}

void Bullet::deactivatePowerBoost()
{
    powerBoosted = false; // Deactivate power boost
}

int Bullet::getkilled()
{
    return killed; // Return the number of enemies killed
}

int Bullet::getgems()
{
    return gems; // Return the number of gems collected
}

int Bullet::getpowerups()
{
    return powerups; // Return the number of powerups collected
}

int Bullet::getusedpowerups()
{
    return usedpowerups; // Return the number of used powerups
}
