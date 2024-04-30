#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height) : QGraphicsRectItem(0, 0, width, height)
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
    Obstacle::s atr = {x,y,width, height};
    return atr;
}
