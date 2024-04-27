#include "rumba.h"

RumbaRC::RumbaRC(int x_in, int y_in, int radius_in, int rotation_in, int speed_in, int detectionLen_in)
    : QGraphicsEllipseItem(0, 0, radius_in * 2, radius_in * 2)
{
    x = x_in;
    y = y_in;
    radius = radius_in;
    rotation = rotation_in;
    speed = speed_in;
    detectionLen = detectionLen_in + radius;

    setTransformOriginPoint(x / 2, y / 2);
    setPos(x, y);
}

RumbaRC::s RumbaRC::getAtributes()
{
    RumbaRC::s atr = {detectionLen, rotation, speed};
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
        rotation -= 5;
        break;
    case Qt::Key_Right:
        rotation += 5;
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

    last_x = x;
    last_y = y;

    x += (speed + detectionLen) * qCos(qDegreesToRadians(rotation));
    y += (speed + detectionLen) * qSin(qDegreesToRadians(rotation));

    setPos(x, y);
}

