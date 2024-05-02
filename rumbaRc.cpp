#include "rumba.h"

RumbaRC::RumbaRC(qreal x_in, qreal y_in, qreal last_x_in, qreal last_y_in, int radius_in, int speed_in , int rotation_in, int detectionLen_in)
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

    setInitialDirection(rotation);

    setTransformOriginPoint(radius, radius);

    setPos(x, y);
}

void RumbaRC::setInitialDirection(qreal initialRotation)
{
    setRotation(initialRotation);
    directionIndicator->setRotation(initialRotation);
    directionIndicator->setPos(radius, radius);
}

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

void RumbaRC::changeDetectionLen(int val)
{
    detectionLen = val;
}

void RumbaRC::stop()
{
    x = last_x;
    y = last_y;

    setPos(x, y);
}

void RumbaRC::move()
{
    x = last_x;
    y = last_y;

    x += speed * qCos(qDegreesToRadians(rotation));
    y += speed * qSin(qDegreesToRadians(rotation));

    setPos(x, y);
}

void RumbaRC::testMove(Qt::Key key)
{
    switch (key) {
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

    directionIndicator->setRotation(rotation);
    directionIndicator->setPos(radius, radius);
    last_x = x;
    last_y = y;

    x += (speed + detectionLen) * qCos(qDegreesToRadians(rotation));
    y += (speed + detectionLen) * qSin(qDegreesToRadians(rotation));

    setPos(x, y);
}

