#include "methods.h"
#include <cmath>

QVector<double> Sweep(QVector<QVector<double> > &matrixA,
                      QVector<double> &vectorB) {
   int n = matrixA.size();
   QVector<double> A(n), B(n), C(n);
   QVector<double> alpha(n), beta(n);
   QVector<double> result(n);

   A[0] = 0;
   B[n - 1] = 0;
   for (int i = 0; i < n; ++i) {
       if (i != n - 1) {
           A[i + 1] = matrixA[i + 1][i];
       }

       if (i != n - 1) {
           B[i] = matrixA[i][i + 1];
       }
       C[i] = matrixA[i][i];
   }

   alpha[1] = -B[0] / C[0];
   beta[1] = vectorB[0] / C[0];
   for (int i = 2; i < n; ++i) {
       alpha[i] = -B[i - 1] / (A[i - 1] * alpha[i - 1] + C[i - 1]);
       beta[i] = (vectorB[i - 1] - A[i - 1] * beta[i - 1]) /
                 (A[i - 1] * alpha[i - 1] + C[i - 1]);
   }

   result[n - 1] = (vectorB[n - 1] - A[n - 1] * beta[n - 1]) /
              (A[n - 1] * alpha[n - 1] + C[n - 1]);
   for (int i = n - 2; i >= 0; --i) {
       result[i] = alpha[i + 1] * result[i + 1] + beta[i + 1];
   }

   return result;
}

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
                                             QVector<double> &t,
                                             int currentApprox) {
    int Nx = x.size();
    int Nk = t.size();
    double sigma = tao / (h * h);

    QVector< QVector<double> > result(Nk, QVector<double>(Nx, 0));

    for (int i = 0; i < Nx; ++i) {
        result[0][i] = std::sin(x[i]);
    }

    for (int k = 1; k < Nk; ++k) {
        for (int i = 1; i < Nx - 1; ++i) {
            result[k][i] = sigma * result[k - 1][i + 1]
                           + (1 - 2 * sigma) * result[k - 1][i]
                           + sigma * result[k - 1][i - 1]
                           + 0.5 * tao *
                             std::exp(-0.5 * t[k - 1]) * std::sin(x[i]);
        }

        if (currentApprox == 1) {
            result[k][0] = result[k][1] - h * std::exp(-0.5 * t[k]);
            result[k][Nx - 1] = result[k][Nx - 2] - h * std::exp(-0.5 * t[k]);
        } else if (currentApprox == 2) {
            result[k][0] = (- 2 * h * std::exp(-0.5 * t[k])
                            + 4 * result[k][1]
                            - result[k][2]) / 3;
            result[k][Nx - 1] = (- 2 * h * std::exp(-0.5 * t[k])
                                 + 4 * result[k][Nx - 2]
                                 - result[k][Nx - 3]) / 3;
        } else if (currentApprox == 3) {
            result[k][0] = (result[k][1] - h * std::exp(-0.5 * t[k])
                            + h * h / 2 * (result[k - 1][0] / tao
                            + 0.5 * std::exp(-0.5 * t[k]) * std::sin(x[0]))) /
                              (1 + h * h / (2 * tao));
            result[k][Nx - 1] = (result[k][Nx - 2] - h * std::exp(-0.5 * t[k])
                + h * h / 2 * (result[k - 1][Nx - 1] / tao
                + 0.5 * std::exp(-0.5 * t[k]) * std::sin(x[Nx - 1]))) /
                  (1 + h * h / (2 * tao));
        }
    }

    return result;
}

QVector<QVector<double> > ImplicitParabolic(double tao, double h,
                                             QVector<double> &x,
                                             QVector<double> &t,
                                             int currentApprox,
                                             double theta) {
    int Nx = x.size();
    int Nk = t.size();
    double sigma = tao / (h * h);

    QVector< QVector<double> > result(Nk, QVector<double>(Nx, 0));

    for (int i = 0; i < Nx; ++i) {
        result[0][i] = std::sin(x[i]);
    }

    QVector< QVector<double> > matrix(Nx, QVector<double>(Nx, 0));
    QVector<double> vector(Nx);

    for (int i = 1; i < matrix.size() - 1; ++i) {
        matrix[i][i - 1] = theta * sigma;
        matrix[i][i] = -(1 + 2 * theta * sigma);
        matrix[i][i + 1] = theta * sigma;
    }

    matrix[0][0] = 1;
    matrix[0][1] = -1;
    matrix[matrix.size() - 1][matrix.size() - 2] = 1;
    matrix[matrix.size() - 1][matrix.size() - 1] = -1;

    for (int k = 1; k < Nk; ++k) {
        for (int i = 1; i < matrix.size() - 1; ++i) {
            vector[i] = -(result[k - 1][i]
              + theta * tao * std::exp(-0.5 * t[k]) *
                std::sin(x[i]) / 2
              + (1 - theta) *
                (tao * std::exp(-0.5 * t[k - 1]) *std::sin(x[i]) / 2
                + sigma * (result[k - 1][i + 1]
                           - 2 * result[k - 1][i] + result[k - 1][i - 1])));
        }

        if (currentApprox == 1) {
            vector[0] = -h * std::exp(-0.5 * t[k]);
            vector[vector.size() - 1] = h * std::exp(-0.5 * t[k]);
        } else if (currentApprox == 2) {
            matrix[0][0] = -3 + matrix[1][0] / (sigma * theta);
            matrix[0][1] = 4 + matrix[1][1] / (sigma * theta);
            matrix[matrix.size() - 1][matrix.size() - 2] = -4
               - matrix[matrix.size() - 2][matrix.size() - 2] / (sigma * theta);
            matrix[matrix.size() - 1][matrix.size() - 1] = 3
               - matrix[matrix.size() - 2][matrix.size() - 1] / (sigma * theta);

            vector[0] = 2 * h * std::exp(-0.5 * t[k])
                        + vector[1] / (sigma * theta);
            vector[vector.size() - 1] = -2 * h * std::exp(-0.5 * t[k])
                                  - vector[vector.size() - 2] / (sigma * theta);
        } else if (currentApprox == 3) {
            matrix[0][0] = 1 + h * h / (2 * tao);
            matrix[0][1] = -1;
            matrix[matrix.size() - 1][matrix.size() - 2] = 1;
            matrix[matrix.size() - 1][matrix.size() - 1] = -1 - h*h / (2 * tao);

            vector[0] = - h * std::exp(-0.5 * t[k])
                        + h * h / 2 * (result[k - 1][0] / tao
                        + 0.5 * std::exp(-0.5 * t[k]) * std::sin(x[0]));
            vector[vector.size() - 1] = h * std::exp(-0.5 * t[k])
                           - h * h / 2 * (result[k - 1][Nx - 1] / tao
                           - 0.5 * std::exp(-0.5 * t[k]) * std::sin(x[Nx - 1]));
        }

        result[k] = Sweep(matrix, vector);
    }

    return result;
}
