/**
 * @file rumba.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief File containing implementation of Rumba
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rumba.h"

/**
 * @brief Construct a new Rumba:: Rumba object
 * This constructor takes incoming data and stores it in the Rumbas dataset and sets its color.
 * Finally, the midpoint of the Rumba is established and its position is set.
 *
 * @param x_in
 * @param y_in
 * @param last_x_in
 * @param last_y_in
 * @param radius_in
 * @param speed_in
 * @param rotationStep_in
 * @param rotation_in
 * @param detectionLen_in
 * @param direction_in
 */
Rumba::Rumba(qreal x_in, qreal y_in, qreal last_x_in, qreal last_y_in, int radius_in, int speed_in, int rotationStep_in, int rotation_in, int detectionLen_in, bool direction_in) : QGraphicsEllipseItem(0, 0, radius_in * 2, radius_in * 2)
{
    x = x_in;
    y = y_in;
    last_x = last_x_in;
    last_y = last_y_in;
    radius = radius_in;
    rotationStep = rotationStep_in;
    rotation = rotation_in;
    direction = direction_in;
    speed = speed_in;
    detectionLen = detectionLen_in + radius;

    setBrush(Qt::blue);

    setTransformOriginPoint(x / 2, y / 2);
    setPos(x, y);
}

/**
 * @brief This method saves incoming data of Rumba to structure for loading.
 *
 * @return Rumba::s
 */
Rumba::s Rumba::getAtributes()
{
    Rumba::s atr = {
        x,
        y,
        last_x,
        last_y,
        radius,
        speed,
        rotationStep,
        rotation,
        detectionLen,
        direction};

    return atr;
}

/**
 * @brief Setter method for setting detection length
 * 
 * @param val 
 */
void Rumba::changeDetectionLen(int val)
{
    detectionLen = val;
}

/**
 * @brief Setter method for setting rotation
 * 
 * @param val 
 */
void Rumba::changeRotation(int val)
{
    rotationStep = val;
}

/**
 * @brief Setter method for setting direction
 * 
 * @param val 
 */
void Rumba::changeDirection(bool val)
{
    direction = val;
}

/**
 * @brief This method moves the Rumba in the intended direction.
 *
 */
void Rumba::move()
{
    x = last_x;
    y = last_y;

    x += speed * cos(rotation * (M_PI / 180));
    y += speed * sin(rotation * (M_PI / 180));

    setPos(x, y);
}

/**
 * @brief This method tests movement by placing rumba in detection length cordinates.
 *
 */
void Rumba::testMove()
{
    last_x = x;
    last_y = y;

    x += (speed + detectionLen) * cos(rotation * (M_PI / 180));
    y += (speed + detectionLen) * sin(rotation * (M_PI / 180));

    setPos(x, y);
}

/**
 * @brief This method changes direction of rotation based on direction flag
 * 
 */
void Rumba::changeDirection()
{
    x = last_x;
    y = last_y;

    setPos(x, y);

    if (direction)
        rotation = (rotation - rotationStep) % 360;
    else
        rotation = (rotation + rotationStep) % 360;
}
