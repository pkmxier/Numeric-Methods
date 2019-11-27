#include <QApplication>
#include <QtDebug>
#include "widget8.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("Решение двумерных уравнений параболического типа");
    Widget8 *lab8 = new Widget8;
    lab8->show();
    return a.exec();
}
