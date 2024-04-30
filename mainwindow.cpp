#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width = ui->graphicsView->width()-10;
    int height = ui->graphicsView->height()-10;

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QColor(0, 0, 30));
    view = new QGraphicsView(scene, this);
    ui->graphicsView->setViewport(view);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start(30); // Update every 30 milliseconds

    // Vytvoření pera s požadovanou barvou a šířkou
    QPen pen(Qt::blue); // Modrá barva ohraničení
    pen.setWidth(4); // Šířka ohraničení

    // Nastavení hranic okolo pohledu
    QGraphicsRectItem *viewBoundaryTop = new QGraphicsRectItem(-5, -5, width + 10, 5);
    viewBoundaryTop->setPen(pen);
    scene->addItem(viewBoundaryTop);

    QGraphicsRectItem *viewBoundaryLeft = new QGraphicsRectItem(-5, -5, 5, height + 10);
    viewBoundaryLeft->setPen(pen);
    scene->addItem(viewBoundaryLeft);

    QGraphicsRectItem *viewBoundaryRight = new QGraphicsRectItem(width, -5, 5, height + 10);
    viewBoundaryRight->setPen(pen);
    scene->addItem(viewBoundaryRight);

    QGraphicsRectItem *viewBoundaryBottom = new QGraphicsRectItem(-5, height, width + 10, 5);
    viewBoundaryBottom->setPen(pen);
    scene->addItem(viewBoundaryBottom);

    this->setStyleSheet("background-color: #2E2E2E;");


    show();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up or event->key() == Qt::Key_W) {
        lastKeyPressed = Qt::Key_Up;
    } else if (event->key() == Qt::Key_Down or event->key() == Qt::Key_S) {
        lastKeyPressed = Qt::Key_Down;
    } else if (event->key() == Qt::Key_Left or event->key() == Qt::Key_A) {
        lastKeyPressed = Qt::Key_Left;
    } else if (event->key() == Qt::Key_Right or event->key() == Qt::Key_D) {
        lastKeyPressed = Qt::Key_Right;
    }
}

void MainWindow::updateScene() {
    // Update Rumba positions and handle collisions
    isActiveR = dynamic_cast<Rumba*>(activeItem);
    isActiveRRC = dynamic_cast<RumbaRC*>(activeItem);
    isActiveO = dynamic_cast<Obstacle*>(activeItem);


    for (auto rumba : rumbas) {
        if (rumba->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton) {
            activeItem = rumba;
            ui->tabWidget->setCurrentIndex(0);
        }

        rumba->setBrush(Qt::blue);
        if (isActiveR != nullptr)
        {
            //set UI on active rumba's values
            isActiveR->setBrush(Qt::red);
            ui->detectionLenDisplay->display(isActiveR->getAtributes().detectionLen);
            ui->rotationDisplay->display(isActiveR->getAtributes().rotationStep);
            ui->dial->setValue(isActiveR->getAtributes().rotationStep);
            ui->horizontalSlider->setValue(isActiveR->getAtributes().detectionLen);

            if (isActiveR->getAtributes().direction)
                ui->radioButton->setChecked(true);
            else ui->radioButton_2->setChecked(true);
        }

        rumba->testMove();
        if (CheckCollision(rumba))
            rumba->changeDirection();
        else
            rumba->move();
    }

    for (auto rumba : rumbasRC) {

        if (rumba->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton) {
            activeItem = rumba;
            ui->tabWidget->setCurrentIndex(1);
        }

        rumba->setBrush(Qt::lightGray);

        if (isActiveRRC != nullptr and rumba == isActiveRRC)
        {
            //set UI on active rc rumba's values
            isActiveRRC->setBrush(Qt::darkGreen);
            ui->detectionLenDisplay->display(isActiveRRC->getAtributes().detectionLen);
            ui->horizontalSlider->setValue(isActiveRRC->getAtributes().detectionLen);
            ui->dial->setValue(isActiveRRC->getAtributes().rotation);
            ui->rotationDisplay->display(isActiveRRC->getAtributes().rotation);

            isActiveRRC->testMove(lastKeyPressed);
            lastKeyPressed = Qt::Key_unknown;

            if (CheckCollision(isActiveRRC) or isActiveRRC->getAtributes().speed == 0)
            {
                isActiveRRC->stop();
            } else
            {
                isActiveRRC->move();
            }
        }
    }

    for (auto obstacle : obstacles) {
        if (obstacle->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton) {
            activeItem = obstacle;
            ui->tabWidget->setCurrentIndex(2);
        }

        obstacle->setBrush(Qt::green);
        if (isActiveO != nullptr)
        {
            //set UI on active obstacle's values
            isActiveO->setBrush(Qt::red);
            ui->horizontalSlider_2->setValue(isActiveO->QGraphicsRectItem::rect().width());
            ui->horizontalSlider_3->setValue(isActiveO->QGraphicsRectItem::rect().height());
        }
    }
}

bool MainWindow::CheckCollision (Rumba *rumba){
    // Check for collisions with scene boundaries

    QList<QGraphicsItem*> colliding_items = rumba->collidingItems();
    return colliding_items.size() != 0;
}

bool MainWindow::CheckCollision (RumbaRC *rumba){
    // Check for collisions with scene boundaries

    QList<QGraphicsItem*> colliding_items = rumba->collidingItems();
    return colliding_items.size() != 0;
}

bool MainWindow::CheckCollision (Obstacle *obstacle){
    // Check for collisions with scene boundaries

    QList<QGraphicsItem*> colliding_items = obstacle->collidingItems();
    return colliding_items.size() != 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//menu for Rumba class

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (!loading)
    {
        if (rumbas.size() < arg1)
        {
            int radius = 25;
            for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + radius * 2)) {
                for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + radius * 2)) {
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
                        continue;
                    }

                };
            };

        } else
        {
            scene->removeItem(rumbas.back());
            rumbas.pop_back();
        }
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
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


//Menu for Obstacle class

/**
* Change width of active obstacle
*/
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
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
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    if (isActiveO != nullptr)
    {
        isActiveO->setRect(isActiveO->QGraphicsRectItem::rect().x(), isActiveO->QGraphicsRectItem::rect().y(), isActiveO->QGraphicsRectItem::rect().width(), value);
        ui->heightObstacleDisplay->display(value);
    }
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    if (!loading)
    {
        if (obstacles.size() < arg1)
        {
            int width = randomGen->bounded(50, 300);
            int height = randomGen->bounded(50, 300);
            for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + width)) {
                for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + height)) {
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
                        continue;
                    }

                };
            };
            ui->spinBox_2->setValue(obstacles.size());
        } else if (obstacles.size() > arg1)
        {
            scene->removeItem(obstacles.back());
            obstacles.pop_back();
        }
    }
}


//Menu for RumbaRC class

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    if (!loading)
    {
        if (rumbasRC.size() < arg1)
        {
            int radius = 25;
            for (int hI = 0; hI < ui->graphicsView->height(); hI = (hI + radius * 2)) {
                for (int wI = 0; wI < ui->graphicsView->width(); wI = (wI + radius * 2)) {
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
                        continue;
                    }

                };
            };

        } else
        {
            scene->removeItem(rumbasRC.back());
            rumbasRC.pop_back();
        }
    }
}






//JSON saving

void MainWindow::setRumbas(std::vector<Rumba::s> v){
    ui->spinBox->setValue(v.size());
    for (Rumba::s rumbaStruct : v) {
        auto [x, y, last_x, last_y, radius, speed, rotationStep, rotation, detectionLen, direction] = rumbaStruct;
        std::cout<< x << " " << y << std::endl;
        auto r = new Rumba(x, y, last_x, last_y, radius, speed, rotationStep, rotation, detectionLen, direction);
        scene->addItem(r);
        rumbas.push_back(r);
    }
}

void MainWindow::setRumbasRC(std::vector<RumbaRC::s> v){
    ui->spinBox_2->setValue(v.size());
    for (RumbaRC::s  rumbaRCStruct: v) {
        auto [x, y, last_x, last_y, radius, speed, rotation, detectionLen] = rumbaRCStruct;
        auto r = new RumbaRC(x, y, last_x, last_y, radius, speed, rotation, detectionLen);
        scene->addItem(r);
        rumbasRC.push_back(r);
    }

}

void MainWindow::setObstacles(std::vector<Obstacle::s> v){
    ui->spinBox_3->setValue(v.size());
    for (Obstacle::s obstacleStruct: v) {
        auto [x, y, width, height] = obstacleStruct;
        auto o = new Obstacle(x, y, width, height);
        scene->addItem(o);
        obstacles.push_back(o);
    }

}


std::vector<Rumba*> MainWindow::getRumbas()
{
    return this->rumbas;
}

std::vector<RumbaRC*> MainWindow::getRumbasRC(){
    return this->rumbasRC;
}
std::vector<Obstacle*> MainWindow::getObstacles(){
    return this->obstacles;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    std::cout << "saving" << std::endl;
    stateManager->saveStateToJson("./test.json", this);
}

MainWindow::otherAtributes MainWindow::getOtherAtr() {
    MainWindow::otherAtributes atr ={
        activeItem,
        lastKeyPressed,
    };
    return atr;
};

void MainWindow::on_pushButton_2_clicked()
{
    loading = true;
    std::cout << "loading" << std::endl;
    stateManager->loadStateFromJson("./test.json", this);
    loading = false;
}

