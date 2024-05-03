/**
 * @file obstacle.h
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief 
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QWidget>

class Obstacle : public QWidget, public QGraphicsRectItem
{
    Q_OBJECT
public:
    // Constructor
    Obstacle(qreal x, qreal y, qreal width, qreal height);
    struct s
    {
        qreal x;
        qreal y;
        qreal width;
        qreal height;
    };
    s getAtributes();

private:
    qreal x;
    qreal y;
    qreal width;
    qreal height;
};

#endif // OBSTACLE_H
