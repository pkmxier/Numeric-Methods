#include "methods.h"

double f(double x, double t) {
    return std::exp(-0.5 * t) * std::sin(x);
}

QVector< QVector<double> > AnalyticSolution(QVector<double> &x,
                                            QVector<double> &t) {
    QVector< QVector<double> > result(t.size(), QVector<double>(x.size(), 0));

    for (int k = 0; k < t.size(); ++k) {
        for (int i = 0; i < x.size(); ++i) {
            result[k][i] = f(x[i], t[k]);
        }
    }

    return result;
}

QVector<QVector<double> > ExplicitParabolic(double tao, double h,
                                            QVector<double> &x,
                                            QVector<double> &t) {
    int Nx = x.size();
    int Nk = t.size();
    double sigma = tao / (h * h);

    QVector< QVector<double> > result(Nk, QVector<double>(Nx, 0));

    for (int i = 0; i < Nx; ++i) {
        result[0][i] = std::sin(t[i]);
    }

    for (int k = 1; k < Nk; ++k) {
        for (int i = 1; i < Nx - 1; ++i) {
            result[k][i] = sigma * (result[k - 1][i + 1] + result[k - 1][i - 1])
                           + (1 - 2 * sigma) * result[k - 1][i]
                           + tao * f(t[k - 1], x[i]);
        }

        result[k][0] = result[k][1] - h * std::exp(-0.5 * t[k]);
        result[k][Nx - 1] = result[k][Nx - 2] + h * std::exp(-0.5 * t[k]);
    }

    return result;
}
