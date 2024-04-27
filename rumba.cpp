#include "rumba.h"

Rumba::Rumba(int x_in, int y_in, int radius_in, int rotation_in , int speed_in, int detectionLen_in)
    : QGraphicsEllipseItem(0, 0, radius_in * 2 , radius_in *2)
{
    x = x_in;
    y = y_in;
    radius = radius_in;
    rotation = rotation_in;
    rotationStep = 35;
    direction = 1;
    speed = speed_in;
    detectionLen = detectionLen_in + radius;
    setBrush(Qt::blue);

    setTransformOriginPoint(x/2, y/2);
    setPos(x, y);
}

Rumba::s Rumba::getAtributes()
{
   Rumba::s atr ={rotationStep, detectionLen, direction};
   return atr;
}

void Rumba::changeDetectionLen(int val){
    detectionLen = val;
}

void Rumba::changeRotation(int val){
    rotationStep = val;
}

void Rumba::changeDirection(bool val){
    direction = val;
}

void Rumba::move()
{
    x = last_x;
    y = last_y;

    x += speed * qCos(qDegreesToRadians(rotation));
    y += speed * qSin(qDegreesToRadians(rotation));

    setPos(x, y);
}

void Rumba::testMove()
{
    // Update the x and y positions based on the current rotation
    last_x = x;
    last_y = y;

    x += (speed + detectionLen) * qCos(qDegreesToRadians(rotation));
    y += (speed + detectionLen) * qSin(qDegreesToRadians(rotation));

    setPos(x, y);
}

void Rumba::changeDirection()
{
    x = last_x;
    y = last_y;

    setPos(x, y);

    if (direction)
        rotation = (rotation - rotationStep) % 360;
    else
        rotation = (rotation + rotationStep) % 360;

}
