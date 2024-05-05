/**
 * @file main.cpp
 * @author Tomáš Hlásenský (xhlase01)
 * @author Michael Babušík (xbabus01)
 * @brief File containing main function
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "mainwindow.h"
#include <QApplication>

/**
 * @brief Entry point of the application.
 * This function initializes the application, sets its style to Fusion, creates the main window,
 * and displays it.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
