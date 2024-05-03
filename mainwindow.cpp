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

/**
 * @brief Destroy the Main Window:: Main Window object and all dynamicly created objects
 *
 */
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

/**
 * @brief This function deletes all items from arrays rumbas, rumbasRC and obstacles and clears the arrays
 *
 */
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

/**
 * @brief This function gets QKeyEvent and check if it's w,a,s,d and passes this information to rumbaRC
 *
 * @param event
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up or event->key() == Qt::Key_W)
    {
        ui->wKey->setFlat(true);
        QTimer::singleShot(100, [this]()
                           { ui->wKey->setFlat(false); });
        ui->aKey->setFlat(false);
        ui->sKey->setFlat(false);
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Up;
    }
    else if (event->key() == Qt::Key_Down or event->key() == Qt::Key_S)
    {
        ui->sKey->setFlat(true);
        QTimer::singleShot(100, [this]()
                           { ui->sKey->setFlat(false); });
        ui->wKey->setFlat(false);
        ui->aKey->setFlat(false);
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Down;
    }
    else if (event->key() == Qt::Key_Left or event->key() == Qt::Key_A)
    {
        ui->aKey->setFlat(true);
        QTimer::singleShot(100, [this]()
                           { ui->aKey->setFlat(false); });
        ui->dKey->setFlat(false);
        lastKeyPressed = Qt::Key_Left;
    }
    else if (event->key() == Qt::Key_Right or event->key() == Qt::Key_D)
    {
        ui->dKey->setFlat(true);
        QTimer::singleShot(100, [this]()
                           { ui->dKey->setFlat(false); });
        ui->aKey->setFlat(false);
        lastKeyPressed = Qt::Key_Right;
    }
}

/**
 * @brief Updates the scene with rumba movements, collisions, and UI interactions.
 * This function iterates through all rumbas, rumbaRCs (remote controlled rumbas), and obstacles in the scene. It performs the following actions:
 * - Checks for left mouse button click: If a rumba, rumbaRC, or obstacle is clicked with the left mouse button, it becomes the active item, and the corresponding tab in the UI is switched.
 * - Updates rumba, rumbaRC and obstacle colors: Sets the brush color of all items and the active on (if any) to red.
 * - Updates UI with active item values: If an item is active (rumba, rumbaRC, or obstacle), its attributes are displayed in the corresponding UI elements (detection length, rotation, speed, size sliders, etc.).
 * - Moves rumbas: Calls the testMove function on all rumbas to simulate movement and handle potential collisions. If a collision occurs, the rumba changes direction. This process is repeated for actual movement if no collision is detected during the test move.
 * - Moves rumbaRCs: Calls the testMove function on all rumbaRCs with the last pressed key as input (for remote control). If a collision occurs or the speed is zero, the rumbaRC stops. Otherwise, it moves and checks for collisions again.
 * - Ignores updates during loading: If the loading flag is set, the function exits without updating the scene.
 */
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

/**
 * @brief Checks for collisions between a rumba and other items in the scene.
 * This function uses rumba->collidingItems() to retrieve a list of all items colliding with the rumba. It then checks if the list is empty and returns the corresponding boolean value.
 *
 * @param rumba
 * @return true
 * @return false
 */
bool MainWindow::CheckCollision(Rumba *rumba)
{
    QList<QGraphicsItem *> colliding_items = rumba->collidingItems();
    return colliding_items.size() != 0;
}

/**
 * @brief Checks for collisions between a rumbaRC and other items in the scene.
 * This function uses rumbaRC->collidingItems() to retrieve a list of all items colliding with the rumbaRC. It then checks if the list is empty and returns the corresponding boolean value.
 *
 * @param rumbaRC
 * @return true
 * @return false
 */
bool MainWindow::CheckCollision(RumbaRC *rumbaRC)
{
    QList<QGraphicsItem *> colliding_items = rumbaRC->collidingItems();
    return colliding_items.size() != 1;
}

/**
 * @brief Checks for collisions between a obstacle and other items in the scene.
 * This function uses obstacle->collidingItems() to retrieve a list of all items colliding with the obstacle. It then checks if the list is empty and returns the corresponding boolean value.
 *
 * @param obstacle
 * @return true
 * @return false
 */
bool MainWindow::CheckCollision(Obstacle *obstacle)
{
    QList<QGraphicsItem *> colliding_items = obstacle->collidingItems();
    return colliding_items.size() != 0;
}

// menu for Rumba class
/**
 * @brief Slot function connected to the "Number of Rumbas" spin box value change.
 * This function checks for the loading flag and exits if true (prevents updates during loading).
 * If the number of rumbas needs to be increased, it creates new Rumba object within grid created with size of a rumba within the scene boundaries and adds them if no collision occurs.
 * Otherwise, it removes the last rumba and updates the UI and internal list.
 *
 * @param arg1 The new value of the spin box.
 */
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

/**
 * @brief This function updates the active rumba's detection length value and UI display.
 *
 * @param value
 */
void MainWindow::on_detectionLenSlider_valueChanged(int value)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeDetectionLen(value);
        ui->detectionLenDisplay->display(value);
    }
}

/**
 * @brief This function updates the active rumba's rotation value and UI display.
 *
 * @param value
 */
void MainWindow::on_dial_valueChanged(int value)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeRotation(value);
        ui->rotationDisplay->display(value);
    }
}

/**
 * @brief This function updates the active rumba's side of rotation.
 *
 * @param checked
 */
void MainWindow::on_radioButton_toggled(bool checked)
{
    if (isActiveR != nullptr)
    {
        isActiveR->changeDirection(checked);
    }
}

// Menu for Obstacle class

/**
 * @brief This function updates the active obstacle's width value and UI display.
 *
 * @param value
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
 * @brief This function updates the active obstacle's height value and UI display.
 *
 * @param value
 */
void MainWindow::on_obstacleHeightSlider_valueChanged(int value)
{
    if (isActiveO != nullptr)
    {

        isActiveO->setRect(
            isActiveO->QGraphicsRectItem::rect().x(), isActiveO->QGraphicsRectItem::rect().y(),
            isActiveO->QGraphicsRectItem::rect().width(),
            value);
        ui->heightObstacleDisplay->display(value);
    }
}

/**
 * @brief Slot function connected to the "Number of Obstacles" spin box value change.
 * This function checks for the loading flag and exits if true (prevents updates during loading).
 * If the number of obstacles needs to be increased, it creates new Obstacle object within grid created with size of a obstacle within the scene boundaries and adds them if no collision occurs.
 * Otherwise, it removes the last obstacle and updates the UI and internal list.
 *
 * @param arg1 The new value of the spin box.
 */
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

/**
 * @brief This function sets the lastKeyPressed variable to Qt::Key_Up to indicate the "Up" movement for the active RumbaRC.
 *
 */
void MainWindow::on_wKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        ui->sKey->setFlat(false);
        lastKeyPressed = Qt::Key_Up;
    }
}

/**
 * @brief This function sets the lastKeyPressed variable to Qt::Key_Down to indicate the "Stop" movement for the active RumbaRC.
 *
 */
void MainWindow::on_sKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        ui->wKey->setFlat(false);
        lastKeyPressed = Qt::Key_Down;
    }
}

/**
 * @brief This function sets the lastKeyPressed variable to Qt::Key_Left to indicate the "left" movement for the active RumbaRC.
 *
 */
void MainWindow::on_aKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        lastKeyPressed = Qt::Key_Left;
    }
}

/**
 * @brief This function sets the lastKeyPressed variable to Qt::Key_Right to indicate the "Right" movement for the active RumbaRC.
 *
 */
void MainWindow::on_dKey_clicked()
{
    if (isActiveRRC != nullptr)
    {
        lastKeyPressed = Qt::Key_Right;
    }
}

/**
 * @brief Slot function connected to the "Number of RumbasRC" spin box value change.
 * This function checks for the loading flag and exits if true (prevents updates during loading).
 * If the number of rumbasRC needs to be increased, it creates new RumbaRC object within grid created with size of a rumbaRC within the scene boundaries and adds them if no collision occurs.
 * Otherwise, it removes the last rumbaRC and updates the UI and internal list.
 *
 * @param arg1 The new value of the spin box.
 */
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

/**
 * @brief Slot function connected to the "New Game" button click.
 * This function calls the newGame function to reset the game state.
 */
void MainWindow::on_newGameBtn_clicked()
{
    newGame();
}

/**
 * @brief Slot function connected to the "Load Game" button click.
 * This function calls newGame to reset, sets the loading flag, loads the game state from a JSON file using stateManager->loadStateFromJson, and then clears the loading flag.
 *
 */
void MainWindow::on_loadBtn_clicked()
{
    newGame();
    loading = true;
    stateManager->loadStateFromJson("./save.json", this);
    loading = false;
}

/**
 * @brief Slot function connected to the "Pause Game" button click.
 * This function toggles the pause state of the game.
 * It uses timer->start(timeStep) for resuming the game and times->stop() for stopping.
 */
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

/**
 * @brief Slot function connected to the "Quit Game" button click.
 * This function closes the application using QApplication::quit.
 */
void MainWindow::on_quitBtn_clicked()
{
    QApplication::quit();
}

// JSON saving

/**
 * @brief Sets the rumbas in the scene based on a vector of Rumba structs Rumba::s.
 * This function iterates through the provided vector and creates new Rumba objects based on the data. It adds them to the scene and the internal rumbas list.
 *
 * @param rV
 */
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

/**
 * @brief Sets the rumbasRC in the scene based on a vector of RumbaRC structs RumbaRC::s.
 * This function iterates through the provided vector and creates new RumbaRC objects based on the data. It adds them to the scene and the internal rumbas list.
 *
 * @param rcV
 */
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

/**
 * @brief Sets the obstacles in the scene based on a vector of Obstacle structs Obstacle::s.
 * This function iterates through the provided vector and creates new Obstacle objects based on the data. It adds them to the scene and the internal obstacles list.
 *
 * @param oV
 */
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

/**
 * @brief Getter function to access the list of Rumba objects in the scene.
 *
 * @return std::vector<Rumba *>
 */
std::vector<Rumba *> MainWindow::getRumbas()
{
    return this->rumbas;
}

/**
 * @brief Getter function to access the list of RumbaRC objects in the scene.
 *
 * @return std::vector<RumbaRC *>
 */
std::vector<RumbaRC *> MainWindow::getRumbasRC()
{
    return this->rumbasRC;
}

/**
 * @brief Getter function to access the list of Obstacle objects in the scene.
 *
 * @return std::vector<Obstacle *>
 */
std::vector<Obstacle *> MainWindow::getObstacles()
{
    return this->obstacles;
}

/**
 * @brief Slot function connected to the "Save Game" button click.
 * This function saves the current game state to a JSON file using stateManager->saveStateToJson.
 */
void MainWindow::on_saveBtn_clicked()
{
    stateManager->saveStateToJson("./save.json", this);
}
