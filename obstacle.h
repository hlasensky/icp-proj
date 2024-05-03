#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>
#include <QWidget>

class Obstacle : public QWidget , public QGraphicsRectItem
{
    Q_OBJECT
public:
    // Constructor
    Obstacle(qreal x, qreal y, qreal width, qreal height);
    struct s{
        qreal x;
        qreal y;
        qreal width;
        qreal height;
    };
    s getAtributes();

private:
    qreal x;
    qreal y;
    qreal width;
    qreal height;
};

#endif // OBSTACLE_H
