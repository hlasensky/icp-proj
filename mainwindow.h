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
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonArray>

#include "rumba.h"
#include "obstacle.h"

class StateManager;

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
    std::vector<Rumba*> getRumbas();
    std::vector<RumbaRC*> getRumbasRC();
    std::vector<Obstacle*> getObstacles();
    struct otherAtributes {
        QWidget* activeItem;
        Qt::Key lastKeyPressed;
    };
    otherAtributes getOtherAtr();
    void setRumbas(std::vector<Rumba::s> v);
    void setRumbasRC(std::vector<RumbaRC::s> v);
    void setObstacles(std::vector<Obstacle::s> v);
    bool loading;


private:
    StateManager *stateManager;

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
    void on_pushButton_clicked(bool checked);
    void on_pushButton_2_clicked();
};

class StateManager {
public:
    StateManager();

    void saveStateToJson(const QString& fileName, MainWindow *perent);
    void loadStateFromJson(const QString& fileName, MainWindow* perent);

private:
    QJsonObject  structToJson( QWidget *obj);

    RumbaRC::s  jsonToRumbaRCStruct(const QJsonObject& jsonObject);
    QJsonObject  structToJson( RumbaRC::s myStruct);

    Rumba::s  jsonToRumbaStruct(const QJsonObject& jsonObject);
    QJsonObject  structToJson( Rumba::s myStruct);

    Obstacle::s jsonToObstacleStruct(const QJsonObject& jsonObject);
    QJsonObject  structToJson( Obstacle::s obstacle);



};
#endif // MAINWINDOW_H
