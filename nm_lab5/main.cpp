#include <QApplication>
#include <QtDebug>
#include "widget5.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationName("Решение уравнений параболического типа");
    Widget5 *lab5 = new Widget5;
    lab5->show();
    return a.exec();
}
