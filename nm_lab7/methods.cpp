#include "methods.h"
#include <cmath>

double f(double x, double y) {
    return x * std::cos(y);
}

QVector< QVector<double> > AnalyticSolution(QVector<double> &x,
                                             QVector<double> &y) {
    QVector< QVector<double> > result(x.size(), QVector<double>(y.size(), 0));

    for (int k = 0; k < x.size(); ++k) {
        for (int i = 0; i < y.size(); ++i) {
            result[k][i] = f(x[k], y[i]);
        }
    }

    return result;
}

double DifferenceNorm(QVector< QVector<double> > &result,
                       QVector< QVector<double> > &prevResult) {
    double error = 0;

    for (int i = 0; i < result.size(); ++i) {
        for (int j = 0; j < result[i].size(); ++j) {
            error = std::max(error, std::fabs(result[i][j] - prevResult[i][j]));
        }
    }

    return error;
}

QVector<QVector<double> > Elliptic(double h_x, double h_y,
                                   QVector<double> &x, QVector<double> &y,
                                   double epsilon) {
    int Nx = x.size();
    int Ny = y.size();

    QVector< QVector<double> > prevResult(Nx, QVector<double>(Ny, 0));
    QVector< QVector<double> > result(Nx, QVector<double>(Ny, 0));

    for (int i = 1; i < Nx - 1; ++i) {
        result[i][0] = x[i];
        result[i][Ny - 1] = 0;

        for (int j = 1; j < Ny - 1; ++j) {
            result[i][j] = result[i][j - 1] - x[i] / (Ny - 2);
        }
    }
    result[0][0] = x[0];
    result[Nx - 1][0] = x[Nx - 1];
    result[0][Ny - 1] = result[Nx - 1][Ny - 1] = 0;

    for (int j = 1; j < Ny - 1; ++j) {
        result[0][j] = result[1][j] - h_x * std::cos(y[j]);
        result[Nx - 1][j] = result[Nx - 2][j] / (1 - h_x);
    }


    double norm = 0;
    int cnt = 0;
    do {
        for (int i = 0; i < Nx; ++i) {
            for (int j = 0; j < Ny; ++j) {
                std::swap(result[i][j], prevResult[i][j]);
            }
        }

        for (int i = 1; i < Nx - 1; ++i) {
            for (int j = 1; j < Ny - 1; ++j) {
                result[i][j] =
                        ((prevResult[i + 1][j] + prevResult[i - 1][j]) / (h_x * h_x) +
                         (prevResult[i][j + 1] + prevResult[i][j - 1]) / (h_y * h_y)) /
                        (2.0 * (1 / (h_x * h_x) + 1 / (h_y * h_y)) - 1);
            }
        }

        for (int i = 0; i < Nx; ++i) {
            result[i][0] = x[i];
            result[i][Ny - 1] = 0;
        }

        for (int j = 1; j < Ny - 1; ++j) {
            result[0][j] = result[1][j] - h_x * std::cos(y[j]);
            result[Nx - 1][j] = result[Nx - 2][j] / (1 - h_x);
        }

        norm = DifferenceNorm(result, prevResult);
        qDebug() << cnt++ << norm;
    } while (norm > epsilon);

    return result;
}
