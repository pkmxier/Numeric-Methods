#include <QApplication>
#include <QtDebug>
#include "widget6.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("Решение уравнений гиперболического типа");
    Widget6 *lab6 = new Widget6;
    lab6->show();
    return a.exec();
}
