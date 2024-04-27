#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <vector>
#include <iostream>
#include <QSpinBox>
#include <QDebug>


#include "rumba.h"
#include "obstacle.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QRandomGenerator *randomGen = QRandomGenerator::global();
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;

    std::vector<Rumba*> rumbas;
    std::vector<RumbaRC*> rumbasRC;
    std::vector<Obstacle*> obstacles;

    Rumba *isActiveR = NULL;
    RumbaRC *isActiveRRC = NULL;
    Obstacle *isActiveO = NULL;
    QWidget* activeItem = nullptr;
    Qt::Key lastKeyPressed = Qt::Key_unknown;

    QTimer *timer;

public slots:
    void updateScene();
    bool CheckCollision (Rumba *r);
    bool CheckCollision(Obstacle *o);
    bool CheckCollision (RumbaRC *rumba);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_spinBox_valueChanged(int arg1);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_spinBox_2_valueChanged(int arg1);
    void on_dial_valueChanged(int value);
    void on_radioButton_toggled(bool checked);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_spinBox_3_valueChanged(int arg1);
};
#endif // MAINWINDOW_H
