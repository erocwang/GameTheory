#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(diagramarea);
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(640,480);
    w.show();
    return a.exec();
}
