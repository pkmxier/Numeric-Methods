#include <QApplication>
#include <QtDebug>
#include "widget7.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("Решение уравнений эллиптического типа");
    Widget7 *lab7 = new Widget7;
    lab7->show();
    return a.exec();
}
