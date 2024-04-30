#include "rumba.h"

Rumba::Rumba(qreal x_in, qreal y_in, qreal last_x_in, qreal last_y_in, int radius_in, int speed_in, int rotationStep_in, int rotation_in, int detectionLen_in, bool direction_in):
    QGraphicsEllipseItem(0, 0, radius_in * 2 , radius_in *2)
{
    x = x_in;
    y = y_in;
    last_x = last_x_in;
    last_y = last_y_in;
    radius = radius_in;
    rotationStep = rotationStep_in;
    rotation = rotation_in;
    direction = direction_in;
    speed = speed_in;
    detectionLen = detectionLen_in + radius;

    setBrush(Qt::blue);


    setTransformOriginPoint(x/2, y/2);
    setPos(x, y);
}

Rumba::s Rumba::getAtributes()
{
   Rumba::s atr ={
    x,
    y,
    last_x,
    last_y,
    radius,
    speed,
    rotationStep,
    rotation,
    detectionLen,
    direction
    };

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
