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
    view = new QGraphicsView(scene, this);
    ui->graphicsView->setViewport(view);

    QGraphicsScene *sceneR = new QGraphicsScene(this);
    QGraphicsView *viewR = new QGraphicsView(sceneR, this);
    ui->graphicsView_rumbas->setViewport(viewR);
    auto o = new Obstacle(300, 100, 250, 450);
    sceneR->addItem(o);
    o->setAcceptedMouseButtons(Qt::LeftButton);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start(100); // Update every 30 milliseconds


    QGraphicsRectItem *sceneBoundary = new QGraphicsRectItem(0, 0, -500,height);
    scene->addItem(sceneBoundary);
    QGraphicsRectItem *sceneBoundary2 = new QGraphicsRectItem(-100, 0, 200+width,-500);
    scene->addItem(sceneBoundary2);
    QGraphicsRectItem *sceneBoundary3 = new QGraphicsRectItem(width+10, 0, 500,height);
    scene->addItem(sceneBoundary3);
    QGraphicsRectItem *sceneBoundary4 = new QGraphicsRectItem(-100, height+10, 200+width,500);
    scene->addItem(sceneBoundary4);

    show();
}

void MainWindow::updateScene() {
    // Update Rumba positions and handle collisions
    for (auto rumba : rumbas) {

        rumba->setBrush(Qt::blue);

        if (isActiveR)
        {
            isActiveR->setBrush(Qt::red);
            ui->detectionLenDisplay->display(isActiveR->getAtributes().detectionLen);
            ui->rotationDisplay->display(isActiveR->getAtributes().rotationStep);

            if (isActiveR->getAtributes().direction)
                ui->radioButton->setChecked(true);
            else ui->radioButton_2->setChecked(true);
        }

        if (rumba->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton) {
            isActiveR = rumba;
        }

        std::cout << isActiveR << std::endl;

        rumba->testMove();
        if (CheckCollision(rumba))
            rumba->changeDirection();
        else
            rumba->move();
    }

    for (auto obstacle : obstacles) {

        obstacle->setBrush(Qt::green);

        if (isActiveO)
            isActiveO->setBrush(Qt::red);

        if (obstacle->isUnderMouse() && QApplication::mouseButtons() & Qt::LeftButton) {
            isActiveO = obstacle;
        }

        std::cout << isActiveO << std::endl;

    }


}

bool MainWindow::CheckCollision (Rumba *rumba){


    // Check for collisions with scene boundaries

    QList<QGraphicsItem*> colliding_items = rumba->collidingItems();

    return colliding_items.size() != 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (rumbas.size() < arg1)
    {
        auto r = new Rumba(100, 100, 25, 45, 10, 10);
        scene->addItem(r);
        r->setAcceptedMouseButtons(Qt::LeftButton);
        rumbas.push_back(r);
    } else
    {
        scene->removeItem(rumbas.back());
        rumbas.pop_back();
    }

}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(isActiveR)
    {
        isActiveR->changeDetectionLen(value);

        ui->detectionLenDisplay->display(value);
    }
}



/**
* Change width of active obstacle
*/
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    if(isActiveO)
        isActiveO->setRect(isActiveO->rect().x(), isActiveO->rect().y(), value, isActiveO->rect().height());
}

/**
* Change height of active obstacle
*/
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    if(isActiveO)
        isActiveO->setRect(isActiveO->rect().x(), isActiveO->rect().y(), isActiveO->rect().width(), value);
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    if (obstacles.size() < arg1)
    {
        auto o = new Obstacle(300, 100, 250, 450);
        scene->addItem(o);
        o->setAcceptedMouseButtons(Qt::LeftButton);
        obstacles.push_back(o);
    } else
    {
        scene->removeItem(obstacles.back());
        obstacles.pop_back();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    cursorPos = event->position();
}

void MainWindow::on_dial_valueChanged(int value)
{
    if(isActiveR)
    {
        isActiveR->changeRotation(value);
        ui->rotationDisplay->display(value);
    }
}


void MainWindow::on_radioButton_toggled(bool checked)
{
    if(isActiveR){
        isActiveR->changeDirection(checked);
    }
}




