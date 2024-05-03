/**
 * @file obstacle.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01) * @brief 
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "obstacle.h"

Obstacle::Obstacle(qreal x, qreal y, qreal width, qreal height) : QGraphicsRectItem(0, 0, width, height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->setPos(x, y);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

Obstacle::s Obstacle::getAtributes()
{
    Obstacle::s atr = {
        this->QGraphicsRectItem::pos().x(),
        this->QGraphicsRectItem::pos().y(),
        this->QGraphicsRectItem::rect().width(),
        this->QGraphicsRectItem::rect().height()};
    return atr;
}
