#include "mainwindow.h"
#include "menu.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    menu m;
    m.show();
    //m.setWindowTitle(QString("Gobang V1.0 beta"));
    /*
    MainWindow w;
    w.setWindowTitle(QString("Gobang V1.0 beta"));
    w.show();
    */
    return a.exec();
}
