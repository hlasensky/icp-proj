/**
 * @file mainwindow.h
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief
 * @version 0.1
 * @date 2024-05-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGlobal>
#include <QMainWindow>
#include <QtWidgets>
#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <vector>
#include <QSpinBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QMenuBar>
#include <QMenu>
#include <QStyleFactory>

#include "rumba.h"
#include "rumbaRC.h"
#include "obstacle.h"

class StateManager;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The main window class for the application.

 * This class serves as the central hub for the application's graphical user interface (GUI) and manages the overall game state. It inherits from QMainWindow and provides various functionalities, including:
 *
 * - Creating and initializing the GUI elements (menu bar, buttons, scene, etc.)
 * - Handling user interactions (key presses, button clicks)
 * - Managing the game loop, including updating the scene, checking for collisions, and applying movement rules
 * - Providing methods for accessing and modifying game data (e.g., Rumba objects, obstacles, game state)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Rumba *> getRumbas();
    std::vector<RumbaRC *> getRumbasRC();
    std::vector<Obstacle *> getObstacles();
    void setRumbas(std::vector<Rumba::s> v);
    void setRumbasRC(std::vector<RumbaRC::s> v);
    void setObstacles(std::vector<Obstacle::s> v);
    bool loading;

private:
    StateManager *stateManager;

    //QRandomGenerator *randomGen = QRandomGenerator::global();
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QMenuBar *menuBar;
    QMenu *gameMenu;
    QAction *pauseAction;
    QAction *newGameAction;
    QAction *saveGameAction;
    QAction *loadGameAction;
    QAction *quitAction;
    QTimer *timer;
    QGraphicsLineItem *viewBoundaryTop;
    QGraphicsLineItem *viewBoundaryLeft;
    QGraphicsLineItem *viewBoundaryRight;
    QGraphicsLineItem *viewBoundaryBottom;

    std::vector<Rumba *> rumbas;
    std::vector<RumbaRC *> rumbasRC;
    std::vector<Obstacle *> obstacles;

    Rumba *isActiveR;
    RumbaRC *isActiveRRC;
    Obstacle *isActiveO;
    QWidget *activeItem;
    Qt::Key lastKeyPressed = Qt::Key_unknown;

    bool isPaused;
    int timeStep;

    void newGame();
    bool CheckCollision(Rumba *r);
    bool CheckCollision(Obstacle *o);
    bool CheckCollision(RumbaRC *rumba);

public slots:
    void updateScene();
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_numOfRumbas_valueChanged(int arg1);
    void on_detectionLenSlider_valueChanged(int value);
    void on_obstacleWidthSlider_valueChanged(int value);
    void on_numOfObstacles_valueChanged(int arg1);
    void on_dial_valueChanged(int value);
    void on_radioButton_toggled(bool checked);
    void on_obstacleHeightSlider_valueChanged(int value);
    void on_numOfRumbasRC_valueChanged(int arg1);
    void on_loadBtn_clicked();
    void on_saveBtn_clicked();
    void on_pauseBtn_clicked();
    void on_quitBtn_clicked();
    void on_newGameBtn_clicked();
    void on_wKey_clicked();
    void on_aKey_clicked();
    void on_sKey_clicked();
    void on_dKey_clicked();
};

/**
 * @brief Class responsible for persisting game state.

 * This class provides functionalities for saving and loading the game state to and from JSON files. It allows the application to resume gameplay from a previous session or store the current game progress for later use.

 * The `StateManager` interacts with the `MainWindow` object to access and update game data during the saving and loading processes.
 */
class StateManager
{
public:
    StateManager();

    int saveStateToJson(const QString &fileName, MainWindow *perent);
    int loadStateFromJson(const QString &fileName, MainWindow *perent);

private:
    QJsonObject structToJson(QWidget *obj);

    RumbaRC::s jsonToRumbaRCStruct(const QJsonObject &jsonObject);
    QJsonObject structToJson(RumbaRC::s myStruct);

    Rumba::s jsonToRumbaStruct(const QJsonObject &jsonObject);
    QJsonObject structToJson(Rumba::s myStruct);

    Obstacle::s jsonToObstacleStruct(const QJsonObject &jsonObject);
    QJsonObject structToJson(Obstacle::s obstacle);
};
#endif // MAINWINDOW_H
