#ifndef METHODS_H
#define METHODS_H
#include <QVector>
#include <iostream>
#include <QDebug>

const double PI = 3.141592653589793238463;

QVector< QVector<double> > AnalyticSolution(QVector<double> &x,
                                             QVector<double> &y);

QVector< QVector<double> > Elliptic(double h_x, double h_y,
                                    QVector<double> &x, QVector<double> &y,
                                    double epsilon);

#endif // METHODS_H
