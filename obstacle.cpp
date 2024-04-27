#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height) : QGraphicsRectItem(0, 0, width, height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->setPos(x, y);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

void Obstacle::changeW(int val){
    width = val;
}

void Obstacle::changeH(int val){
    height = val;
}

