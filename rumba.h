#ifndef RUMBA_H
#define RUMBA_H

#include <QWidget>
#include <QGraphicsEllipseItem>
#include <iostream>
#include <QPen>
#include <QKeyEvent>

class Rumba : public QWidget , public QGraphicsEllipseItem{
Q_OBJECT
public:
    Rumba(int x, int y, int radius, int rotation , int speed, int detectionLen);

    void move();
    void testMove();
    void changeDirection();
    void changeDetectionLen(int val);
    void changeRotation(int val);
    void changeDirection(bool val);

protected:
    int x;
    int y;
    int last_x;
    int last_y;
    int radius;
    int speed;
    int rotationStep;

    int rotation;
    int detectionLen;
    bool direction;

    struct s {
        int rotationStep;
        int detectionLen;
        bool direction;
    };

public:
    s getAtributes();

};

class RumbaRC : public QWidget , public QGraphicsEllipseItem {
        Q_OBJECT
    public:
        RumbaRC(int x, int y, int radius, int rotation , int speed, int detectionLen);
        
        void move();
        void testMove(Qt::Key key);
        void changeDirection();
        void changeDetectionLen(int val);
        void stop();

    protected:
        int x;
        int y;
        int last_x;
        int last_y;
        int radius;
        int speed;
        int rotation;
        int detectionLen;

        struct s {
            int detectionLen;
            int rotation;
            int speed;
        };

    public:
        s getAtributes();


};

class RumbaAuto : public Rumba{
public:
    void rotate();
};

#endif // RUMBA_H
