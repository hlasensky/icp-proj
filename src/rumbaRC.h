/**
 * @file rumbaRC.h
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief File containing header of RumbaRC
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RUMBARC_H
#define RUMBARC_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QGradient>
#include <math.h>

/**
 * @brief Class representing a RumbaRC object.
 * This class inherits from QWidget and QGraphicsEllipseItem.
 * It encapsulates class methods, a structure, and protected data related to the RumbaRC.
 * It represents a robotic device capable of movement and rotation with WASD keys.
 *
 */
class RumbaRC : public QWidget, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    RumbaRC(qreal x, qreal y, qreal last_x, qreal last_y, int radius, int speed, int rotation, int detectionLen);
    ~RumbaRC();
    void move();
    void testMove(Qt::Key key);
    void changeDirection();
    void changeDetectionLen(int val);
    void stop();
    void updateDirectionIndicator();
    void setDirection();
    struct s
    {
        qreal x;
        qreal y;
        qreal last_x;
        qreal last_y;
        int radius;
        int speed;
        int rotation;
        int detectionLen;
    };
    s getAtributes();

protected:
    qreal x;
    qreal y;
    qreal last_x;
    qreal last_y;
    int radius;
    int speed;
    int rotation;
    int detectionLen;
    QGraphicsLineItem *directionIndicator;
};

#endif // RUMBARC_H
