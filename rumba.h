/**
 * @file rumba.h
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief 
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

/**
 * @brief TODO
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

public:
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
};

#endif // RUMBA_H
