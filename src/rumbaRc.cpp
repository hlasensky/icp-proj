/**
 * @file rumbaRc.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief File containing implementation of RumbaRC
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rumbaRC.h"

/**
 * @brief Construct a new Rumba RC:: Rumba RC object
 * This constructor takes incoming data and stores it in the Rumbas RC dataset and sets its color.
 * It creates a Direction Indicator (DI) as a new object for the Rumba RC, indicating the direction to the player.
 * The DI is set as a black, 4-pixel-wide line with a ZValue of 1.
 * Afterwards, the function 'setDirectionIndicator' is called to determine the correct direction of the indicator.
 * Finally, the midpoint of the Rumba RC is established and its position is set.
 *
 * @param x_in
 * @param y_in
 * @param last_x_in
 * @param last_y_in
 * @param radius_in
 * @param speed_in
 * @param rotation_in
 * @param detectionLen_in
 */
RumbaRC::RumbaRC(qreal x_in, qreal y_in, qreal last_x_in, qreal last_y_in, int radius_in, int speed_in, int rotation_in, int detectionLen_in)
    : QGraphicsEllipseItem(0, 0, radius_in * 2, radius_in * 2)
{
    x = x_in;
    y = y_in;
    last_x = last_x_in;
    last_y = last_y_in;
    radius = radius_in;
    rotation = rotation_in;
    speed = speed_in;
    detectionLen = detectionLen_in + radius;

    directionIndicator = new QGraphicsLineItem(0, 0, radius, 0, this);
    directionIndicator->setPen(QPen(Qt::black, 4));
    directionIndicator->setZValue(1);

    setDirection();

    setTransformOriginPoint(radius, radius);

    setPos(x, y);
}


/**
 * @brief This method sets position and rotation of DI.
 *
 */
void RumbaRC::setDirection()
{
    directionIndicator->setRotation(rotation);
    directionIndicator->setPos(radius, radius);
}

/**
 * @brief This method saves incoming data of Rumba RC to structure for loading.
 *
 * @return RumbaRC::s
 */
RumbaRC::s RumbaRC::getAtributes()
{
    RumbaRC::s atr = {
        x,
        y,
        last_x,
        last_y,
        radius,
        speed,
        rotation,
        detectionLen,
    };
    return atr;
}

/**
 * @brief This method sets detection length for Rumba RC.
 *
 * @param val
 */
void RumbaRC::changeDetectionLen(int val)
{
    detectionLen = val;
}

/**
 * @brief This method stops the movement of Rumba RC.
 *
 */
void RumbaRC::stop()
{
    x = last_x;
    y = last_y;

    setPos(x, y);
}

/**
 * @brief This method moves the RC Rumba in the intended direction.
 *
 */
void RumbaRC::move()
{
    x = last_x;
    y = last_y;

    x += speed * cos(rotation * (M_PI / 180));
    y += speed * sin(rotation * (M_PI / 180));

    setPos(x, y);
}

/**
 * @brief This method tests movement due to collision.
 * It processes incoming keys, if it's a rotation key, it adjusts the rotation; if it's a movement key, it sets the speed.
 * Subsequently, it updates the Direction Indicator (DI) and sets the coordinates.
 *
 * @param key
 */
void RumbaRC::testMove(Qt::Key key)
{
    switch (key)
    {
    case Qt::Key_Left:
        rotation -= 10;
        break;
    case Qt::Key_Right:
        rotation += 10;
        break;
    case Qt::Key_Up:
        speed = 5;
        break;
    case Qt::Key_Down:
        speed = 0;
        break;
    default:
        // Ignore other keys
        break;
    }

    setDirection();
    last_x = x;
    last_y = y;

    x += (speed + detectionLen) * cos(rotation * (M_PI / 180));
    y += (speed + detectionLen) * sin(rotation * (M_PI / 180));

    setPos(x, y);
}

/**
 * @brief Destroy the Rumba RC:: Rumba RC object
 * This destructor deletes DI object
 *
 */
RumbaRC::~RumbaRC()
{
    delete directionIndicator;
}
