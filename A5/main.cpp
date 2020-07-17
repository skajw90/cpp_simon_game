#include "mainwindow.h"
#include <QApplication>
#include "simonmodel.h"
#include <random>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // initialize a one-time seed from the hardware entropy pool
    SimonModel s{std::random_device{}()};
    MainWindow w{s};
    w.show();

    return a.exec();
}
