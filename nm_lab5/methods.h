#ifndef METHODS_H
#define METHODS_H
#include <QVector>
#include <iostream>
#include <QDebug>

const double PI = 3.141592653589793238463;

QVector<double> Sweep(QVector<QVector<double> > &matrixA,
                      QVector<double> &vectorB);

QVector< QVector<double> > AnalyticSolution(QVector<double> &x,
                                             QVector<double> &t);

QVector< QVector<double> > ExplicitParabolic(double tao, double h,
                                              QVector<double> &x,
                                              QVector<double> &t,
                                              int currentApprox);

QVector< QVector<double> > ImplicitParabolic(double tao, double h,
                                              QVector<double> &x,
                                              QVector<double> &t,
                                              int currentApprox,
                                              double theta);

#endif // METHODS_H
