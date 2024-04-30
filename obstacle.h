#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QWidget>

class Obstacle : public QWidget , public QGraphicsRectItem
{
    Q_OBJECT
public:
    // Constructor
    Obstacle(int x, int y, int width, int height);
    struct s{
        qreal x;
        qreal y;
        int width;
        int height;
    };
    s getAtributes();

private:
    qreal x;
    qreal y;
    int width;
    int height;
};

#endif // OBSTACLE_H
