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
    Obstacle::s atr = {this->QGraphicsRectItem::pos().x(),
                       this->QGraphicsRectItem::pos().y(),
        this->QGraphicsRectItem::rect().width(),
        this->QGraphicsRectItem::rect().height()
    };
    return atr;
}
