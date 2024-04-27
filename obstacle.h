#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>

class Obstacle : public QGraphicsRectItem
{
public:
    // Constructor
    Obstacle(int x, int y, int width, int height);
    void changeW(int val);
    void changeH(int val);

private:
    int x;
    int y;
    int width;
    int height;
};

#endif // OBSTACLE_H
