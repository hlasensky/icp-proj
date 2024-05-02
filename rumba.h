#ifndef RUMBA_H
#define RUMBA_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QPen>
#include <QKeyEvent>
#include <QGraphicsLineItem>
#include <QGradient>

class Rumba : public QWidget , public QGraphicsEllipseItem{
Q_OBJECT
public:
    Rumba(qreal x, qreal y, qreal last_x, qreal last_y, int radius, int speed, int rotationStep, int rotation , int detectionLen, bool direction);

    void move();
    void testMove();
    void changeDirection();
    void changeDetectionLen(int val);
    void changeRotation(int val);
    void changeDirection(bool val);



protected:
    qreal x;
    qreal y;
    qreal last_x;
    qreal last_y;
    int radius;
    int speed;
    int rotationStep;

    int rotation;
    int detectionLen;
    bool direction;



public:
    struct s {
        qreal x;
        qreal y;
        qreal last_x;
        qreal last_y;
        int radius;
        int speed;
        int rotationStep;
        int rotation;
        int detectionLen;
        bool direction;
    };
    s getAtributes();


};

class RumbaRC : public QWidget , public QGraphicsEllipseItem {
        Q_OBJECT
    public:
        RumbaRC(qreal x, qreal y, qreal last_x, qreal last_y, int radius, int speed , int rotation, int detectionLen);

        void move();
        void testMove(Qt::Key key);
        void changeDirection();
        void changeDetectionLen(int val);
        void stop();
        void updateDirectionIndicator();
        void setInitialDirection(qreal initialRotation);
        struct s {
            qreal x;
            qreal y;
            qreal last_x;
            qreal last_y;
            int radius;
            int speed;
            int rotation;
            int detectionLen;
        };
        s getAtributes();

    protected:
        qreal x;
        qreal y;
        qreal last_x;
        qreal last_y;
        int radius;
        int speed;
        int rotation;
        int detectionLen;
        QGraphicsLineItem *directionIndicator;

};

#endif // RUMBA_H
