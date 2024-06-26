/**
 * @file rumba.h
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief File containing header of Rumba
 * @version 0.1
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef RUMBA_H
#define RUMBA_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QPen>
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QGradient>
#include <math.h>

/**
 * @brief Class representing a Rumba object.
 * This class inherits from QWidget and QGraphicsEllipseItem.
 * It encapsulates class methods, a structure, and protected data related to the Rumba.
 *
 */
class Rumba : public QWidget, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Rumba(qreal x, qreal y, qreal last_x, qreal last_y, int radius, int speed, int rotationStep, int rotation, int detectionLen, bool direction);

    void move();
    void testMove();
    void changeDirection();
    void changeDetectionLen(int val);
    void changeRotation(int val);
    void changeDirection(bool val);
    struct s
    {
        qreal x;
        qreal y;
        qreal last_x;
        qreal last_y;
        int radius;
        int speed;
        int rotationStep;
        int rotation;
        int detectionLen;
        bool direction;
    };
    s getAtributes();

protected:
    qreal x;
    qreal y;
    qreal last_x;
    qreal last_y;
    int radius;
    int speed;
    int rotationStep;

    int rotation;
    int detectionLen;
    bool direction;
};

#endif // RUMBA_H
