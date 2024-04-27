#include "rumba.h"

void RumbaAuto::rotate()
{
    x += (speed + detectionLen) * qCos(qDegreesToRadians(rotation + 180));
    y += (speed + detectionLen) * qSin(qDegreesToRadians(rotation + 180));

    setPos(x, y);

    if (direction)
        rotation = (rotation - rotationStep) % 360;
    else
        rotation = (rotation + rotationStep) % 360;

}
