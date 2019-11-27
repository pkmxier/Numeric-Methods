#ifndef METHODS_H
#define METHODS_H
#include <QVector>
#include <iostream>
#include <QDebug>

enum Method {
    VariableDirectionMethod,
    FractionalStepMethod
};

const double PI = 3.141592653589793238463;

QVector<double> Sweep(QVector<QVector<double> > &matrixA,
                      QVector<double> &vectorB);

QVector< QVector< QVector<double> > > AnalyticSolution(
        QVector<double> &x, QVector<double> &y, QVector<double> &t, double mu);

QVector< QVector< QVector<double> > > Parabolic2D(
        QVector<double> &x, QVector<double> &y, QVector<double> &t,
        double h_x, double h_y, double tau, double a, double b, double mu, Method method);

#endif // METHODS_H
