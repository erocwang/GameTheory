#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(diagramarea);
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1280,960);
    w.show();
    return a.exec();
}
