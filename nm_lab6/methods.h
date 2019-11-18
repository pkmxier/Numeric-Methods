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

QVector< QVector<double> > ExplicitHyperbolic(double tao, double h,
                                               QVector<double> &x,
                                               QVector<double> &t,
                                               int currentBorderApprox,
                                               int currentTimeApprox);

QVector< QVector<double> > ImplicitHyperbolic(double tao, double h,
                                               QVector<double> &x,
                                               QVector<double> &t,
                                               int currentBorderApprox,
                                               int currentTimeApprox);

#endif // METHODS_H
