/**
 * @file mainwindow.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief 
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "mainwindow.h"
#include "./ui_mainwindow.h"

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isActiveR = nullptr;
    isActiveRRC = nullptr;
    isActiveO = nullptr;
    activeItem = nullptr;

    // Create Bar menu
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Create a "Game" menu
    gameMenu = new QMenu("Game", this);
    menuBar->addMenu(gameMenu);

    pauseAction = new QAction("Pause Game", this);
    gameMenu->addAction(pauseAction);
    connect(pauseAction, &QAction::triggered, this, &MainWindow::on_pauseBtn_clicked);

    newGameAction = new QAction("New Game", this);
    gameMenu->addAction(newGameAction);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    saveGameAction = new QAction("Save Game", this);
    gameMenu->addAction(saveGameAction);
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::on_saveBtn_clicked);

    loadGameAction = new QAction("Load Game", this);
    gameMenu->addAction(loadGameAction);
    connect(saveGameAction, &QAction::triggered, this, &MainWindow::on_loadBtn_clicked);

    gameMenu->addSeparator();

    quitAction = new QAction("Quit", this);
    gameMenu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, qApp, QApplication::quit);

    // setting all thats needed for window, scene and view
    int width = ui->graphicsView->width() - 10;
    int height = ui->graphicsView->height() - 10;

    setFixedSize(1330, 700);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QColor(0, 0, 30));
    view = new QGraphicsView(scene, this);
    ui->graphicsView->setViewport(view);

    loading = false;
    isPaused = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timeStep = 30;
    timer->start(timeStep); // Update every 30 milliseconds

    QPen pen(Qt::blue);
    pen.setWidth(4);

    // Set all boundaries
    viewBoundaryTop = new QGraphicsLineItem(-3, -3, width + 10, -3, nullptr);
    viewBoundaryTop->setPen(pen);
    scene->addItem(viewBoundaryTop);

    viewBoundaryLeft = new QGraphicsLineItem(-3, -3, -3, height + 10, nullptr);
    viewBoundaryLeft->setPen(pen);
    scene->addItem(viewBoundaryLeft);

    viewBoundaryRight = new QGraphicsLineItem(width + 5, -5, width + 5, height + 10, nullptr);
    viewBoundaryRight->setPen(pen);
    scene->addItem(viewBoundaryRight);

    viewBoundaryBottom = new QGraphicsLineItem(-5, height + 4, width + 5, height + 4, nullptr);
    viewBoundaryBottom->setPen(pen);
    scene->addItem(viewBoundaryBottom);

    show();
}

void MainWindow::newGame()
{
    loading = true;
    activeItem = nullptr;

    for (auto r : rumbas)
    {
        scene->removeItem(r);
        delete r;
    }
    rumbas.clear();
    ui->numOfRumbas->setValue(0);

    for (auto rc : rumbasRC)
    {
        scene->removeItem(rc);
        delete rc;
    }
    rumbasRC.clear();
    ui->numOfRumbasRC->setValue(0);

    for (auto o : obstacles)
    {
        scene->removeItem(o);
        delete o;
    }
    obstacles.clear();
    ui->numOfObstacles->setValue(0);

    loading = false;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up or event->key() == Qt::Key_W) {
        ui->wKey->setFlat(true);
        QTimer::singleShot(100, [this](){ ui->wKey->setFlat(false); });
        ui->aKey->setFlat(false);
        ui->sKey->setFlat(false);
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Up;
    } else if (event->key() == Qt::Key_Down or event->key() == Qt::Key_S) {
        ui->sKey->setFlat(true);
        QTimer::singleShot(100, [this](){ ui->sKey->setFlat(false); });
        ui->wKey->setFlat(false);
        ui->aKey->setFlat(false);
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Down;
    } else if (event->key() == Qt::Key_Left or event->key() == Qt::Key_A) {
        ui->aKey->setFlat(true);
        QTimer::singleShot(100, [this](){ ui->aKey->setFlat(false); });
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Left;
    } else if (event->key() == Qt::Key_Right or event->key() == Qt::Key_D) {
        ui->dKey->setFlat(true);
        QTimer::singleShot(100, [this](){ ui->dKey->setFlat(false); });
        ui->aKey->setFlat(false);
        lastKeyPressed = Qt::Key_Right;
    }
}

void MainWindow::updateScene()
{
    if (loading)
        return;

    // Update Rumba positions and handle collisions
    isActiveR = dynamic_cast<Rumba *>(activeItem);
    isActiveRRC = dynamic_cast<RumbaRC *>(activeItem);
    isActiveO = dynamic_cast<Obstacle *>(activeItem);

    for (auto rumba : rumbas)
    {
        if (rumba->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton)
        {
            activeItem = rumba;
            ui->tabWidget->setCurrentIndex(0);
        }

        rumba->setBrush(Qt::blue);
        if (isActiveR != nullptr)
        {
            // set UI on active rumba's values
            isActiveR->setBrush(Qt::red);
            ui->detectionLenDisplay->display(isActiveR->getAtributes().detectionLen);
            ui->rotationDisplay->display(isActiveR->getAtributes().rotationStep);
            ui->dial->setValue(isActiveR->getAtributes().rotationStep);
            ui->detectionLenSlider->setValue(isActiveR->getAtributes().detectionLen);

            if (isActiveR->getAtributes().direction)
                ui->radioButton->setChecked(true);
            else
                ui->radioButton_2->setChecked(true);
        }

        rumba->testMove();
        if (CheckCollision(rumba))
            rumba->changeDirection();
        else
        {
            rumba->move();
            if (CheckCollision(rumba))
                rumba->changeDirection();
        }
    }

    for (auto rumba : rumbasRC)
    {

        if (rumba->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton)
        {
            activeItem = rumba;
            ui->tabWidget->setCurrentIndex(1);
        }

        rumba->setBrush(Qt::lightGray);

        if (isActiveRRC != nullptr and rumba == isActiveRRC)
        {
            // set UI on active rc rumba's values
            isActiveRRC->setBrush(Qt::darkGreen);
            ui->detectionLenDisplay->display(isActiveRRC->getAtributes().detectionLen);
            ui->detectionLenSlider->setValue(isActiveRRC->getAtributes().detectionLen);
            ui->dial->setValue(isActiveRRC->getAtributes().rotation);
            ui->rotationDisplay->display(isActiveRRC->getAtributes().rotation);

            isActiveRRC->testMove(lastKeyPressed);
            lastKeyPressed = Qt::Key_unknown;

            if (CheckCollision(isActiveRRC) or isActiveRRC->getAtributes().speed == 0)
            {
                isActiveRRC->stop();
            }
            else
            {
                isActiveRRC->move();
                if (CheckCollision(isActiveRRC))
                    isActiveRRC->stop();
            }
        }
    }

    for (auto obstacle : obstacles)
    {
        if (obstacle->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton)
        {
            activeItem = obstacle;
            ui->tabWidget->setCurrentIndex(2);
        }

        obstacle->setBrush(Qt::green);
        if (isActiveO != nullptr)
        {
            // set UI on active obstacle's values
            isActiveO->setBrush(Qt::red);
            ui->obstacleWidthSlider->setValue(isActiveO->QGraphicsRectItem::rect().width());
            ui->obstacleHeightSlider->setValue(isActiveO->QGraphicsRectItem::rect().height());
        }
    }
}

bool MainWindow::CheckCollision(Rumba *rumba)
{
    // Check for collisions with scene boundaries

    QList<QGraphicsItem *> colliding_items = rumba->collidingItems();
    return colliding_items.size() != 0;
}

bool MainWindow::CheckCollision(RumbaRC *rumba)
{
    // Check for collisions with scene boundaries

    QList<QGraphicsItem *> colliding_items = rumba->collidingItems();
    return colliding_items.size() != 1;
}

bool MainWindow::CheckCollision(Obstacle *obstacle)
{
    // Check for collisions with scene boundaries

    QList<QGraphicsItem *> colliding_items = obstacle->collidingItems();
    return colliding_items.size() != 0;
}

MainWindow::~MainWindow()
{
    newGame();
    delete scene;
    delete view;
    delete menuBar;
    delete gameMenu;
    delete pauseAction;
    delete newGameAction;
    delete saveGameAction;
    delete loadGameAction;
    delete quitAction;
    delete timer;
    delete viewBoundaryTop;
    delete viewBoundaryLeft;
    delete viewBoundaryRight;
    delete viewBoundaryBottom;
    delete ui;
}

// menu for Rumba class

void MainWindow::on_numOfRumbas_valueChanged(int arg1)
{
    if (loading)
        return;

    if (rumbas.size() < arg1)
    {
        int radius = 20;
        for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + radius * 2))
        {
            for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + radius * 2))
            {
                auto r = new Rumba(wI, hI, 0, 0, radius, 5, 10, 45, 10, 1);
                scene->addItem(r);
                if (!CheckCollision(r))
                {
                    r->setAcceptedMouseButtons(Qt::LeftButton);
                    rumbas.push_back(r);
                    return;
                }
                else
                {
                    scene->removeItem(r);
                    delete r;
                }
            };
        };
    }
    else
    {
        auto last = rumbas.back();
        if (last == activeItem)
            activeItem = nullptr;
        scene->removeItem(last);
        delete last;
        rumbas.pop_back();
    }
}

void MainWindow::on_detectionLenSlider_valueChanged(int value)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeDetectionLen(value);
        ui->detectionLenDisplay->display(value);
    }
}

void MainWindow::on_dial_valueChanged(int value)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeRotation(value);
        ui->rotationDisplay->display(value);
    }
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeDirection(checked);
    }
}

// Menu for Obstacle class

/**
 * Change width of active obstacle
 */
void MainWindow::on_obstacleWidthSlider_valueChanged(int value)
{
    if (isActiveO != nullptr)
    {
        isActiveO->setRect(isActiveO->QGraphicsRectItem::rect().x(), isActiveO->QGraphicsRectItem::rect().y(), value, isActiveO->QGraphicsRectItem::rect().height());
        ui->widthObstacleDisplay->display(value);
    }
}

/**
 * Change height of active obstacle
 */
void MainWindow::on_obstacleHeightSlider_valueChanged(int value)
{
    if (isActiveO != nullptr)
    {

        isActiveO->setRect(isActiveO->QGraphicsRectItem::rect().x(), isActiveO->QGraphicsRectItem::rect().y(), isActiveO->QGraphicsRectItem::rect().width(), value);
        ui->heightObstacleDisplay->display(value);
    }
}

void MainWindow::on_numOfObstacles_valueChanged(int arg1)
{
    if (loading)
        return;

    if (obstacles.size() < arg1)
    {
        int width = randomGen->bounded(ui->obstacleWidthSlider->minimum(), ui->obstacleWidthSlider->maximum());
        int height = randomGen->bounded(ui->obstacleHeightSlider->minimum(), ui->obstacleHeightSlider->maximum());
        for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + width))
        {
            for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + height))
            {
                auto o = new Obstacle(wI, hI, width, height);
                scene->addItem(o);
                if (!CheckCollision(o))
                {
                    o->setAcceptedMouseButtons(Qt::LeftButton);
                    obstacles.push_back(o);
                    return;
                }
                else
                {
                    scene->removeItem(o);
                    delete o;
                }
            };
        };
        ui->numOfObstacles->setValue(obstacles.size());
    }
    else if (obstacles.size() > arg1)
    {
        auto last = obstacles.back();
        if (last == activeItem)
            activeItem = nullptr;
        scene->removeItem(last);
        delete last;
        obstacles.pop_back();
    }
}

// Menu for RumbaRC class

void MainWindow::on_wKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        ui->sKey->setFlat(false);
        lastKeyPressed = Qt::Key_Up;
    }
}

void MainWindow::on_sKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        ui->wKey->setFlat(false);
        lastKeyPressed = Qt::Key_Down;
    }
}


void MainWindow::on_aKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        lastKeyPressed = Qt::Key_Left;
    }
}


void MainWindow::on_dKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        lastKeyPressed = Qt::Key_Right;
    }
}

void MainWindow::on_numOfRumbasRC_valueChanged(int arg1)
{
    if (loading)
        return;

    if (rumbasRC.size() < arg1)
    {
        int radius = 20;
        for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + radius * 2))
        {
            for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + radius * 2))
            {
                auto r = new RumbaRC(wI, hI, 0, 0, radius, 0, 0, 10);
                scene->addItem(r);
                if (!CheckCollision(r))
                {
                    r->setAcceptedMouseButtons(Qt::LeftButton);
                    rumbasRC.push_back(r);
                    return;
                }
                else
                {
                    scene->removeItem(r);
                    delete r;
                }
            };
        };
    }
    else
    {
        auto last = rumbasRC.back();
        if (last == activeItem)
            activeItem = nullptr;
        scene->removeItem(last);
        delete last;
        rumbasRC.pop_back();
    }
}

void MainWindow::on_newGameBtn_clicked()
{
    newGame();
}

void MainWindow::on_loadBtn_clicked()
{
    newGame();
    loading = true;
    stateManager->loadStateFromJson("./test.json", this);
    loading = false;
}

void MainWindow::on_pauseBtn_clicked()
{
    if (isPaused)
    {
        timer->start(timeStep);
        isPaused = false;
        pauseAction->setText("Pause Game");
        ui->pauseBtn->setText("Pause");
    }
    else
    {
        timer->stop();
        isPaused = true;
        pauseAction->setText("Resume Game");
        ui->pauseBtn->setText("Resume");
    }
}

void MainWindow::on_quitBtn_clicked()
{
    QApplication::quit();
}

// JSON saving

void MainWindow::setRumbas(std::vector<Rumba::s> rV)
{
    ui->numOfRumbas->setValue(rV.size());
    for (Rumba::s rumbaStruct : rV)
    {
        auto [x, y, last_x, last_y, radius, speed, rotationStep, rotation, detectionLen, direction] = rumbaStruct;
        auto r = new Rumba(x, y, last_x, last_y, radius, speed, rotationStep, rotation, detectionLen, direction);
        scene->addItem(r);
        rumbas.push_back(r);
    }
}

void MainWindow::setRumbasRC(std::vector<RumbaRC::s> rcV)
{
    ui->numOfRumbasRC->setValue(rcV.size());
    for (RumbaRC::s rumbaRCStruct : rcV)
    {
        auto [x, y, last_x, last_y, radius, speed, rotation, detectionLen] = rumbaRCStruct;
        auto r = new RumbaRC(x, y, last_x, last_y, radius, speed, rotation, detectionLen);
        scene->addItem(r);
        rumbasRC.push_back(r);
    }
}

void MainWindow::setObstacles(std::vector<Obstacle::s> oV)
{
    ui->numOfObstacles->setValue(oV.size());
    for (Obstacle::s obstacleStruct : oV)
    {
        auto [x, y, width, height] = obstacleStruct;
        auto o = new Obstacle(x, y, width, height);
        scene->addItem(o);
        obstacles.push_back(o);
    }
}

std::vector<Rumba *> MainWindow::getRumbas()
{
    return this->rumbas;
}

std::vector<RumbaRC *> MainWindow::getRumbasRC()
{
    return this->rumbasRC;
}
std::vector<Obstacle *> MainWindow::getObstacles()
{
    return this->obstacles;
}

void MainWindow::on_saveBtn_clicked()
{
    stateManager->saveStateToJson("./test.json", this);
}

MainWindow::otherAtributes MainWindow::getOtherAtr()
{
    MainWindow::otherAtributes atr = {
        activeItem,
        lastKeyPressed,
    };
    return atr;
};
