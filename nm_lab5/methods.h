#ifndef METHODS_H
#define METHODS_H
#include <QVector>
#include <iostream>
#include <QDebug>

const double PI = 3.141592653589793238463;

QVector< QVector<double> > AnalyticSolution(QVector<double> &x,
                                            QVector<double> &t);

QVector< QVector<double> > ExplicitParabolic(double tao, double h,
                                             QVector<double> &x,
                                             QVector<double> &t,
                                             int currentApprox);

#endif // METHODS_H
