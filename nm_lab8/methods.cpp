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

double func(double x, double y, double t, double a, double b, double mu) {
    return std::sin(x) * std::sin(y) * (mu * std::cos(mu * t) + (a + b) * std::sin(mu * t));
}

double f(double x, double y, double t, double mu) {
    return std::sin(x) * std::sin(y) * std::sin(mu * t);
}

QVector<QVector<QVector<double> > > AnalyticSolution(
        QVector<double> &x, QVector<double> &y, QVector<double> &t, double mu) {
    QVector<QVector<QVector<double> > > result(
                t.size(), QVector< QVector<double> >(
                    x.size(), QVector<double>(
                        y.size(), 0)));

    for (int k = 0; k < t.size(); ++k) {
        for (int i = 0; i < x.size(); ++i) {
            for (int j = 0; j < y.size(); ++j) {
                result[k][i][j] = f(x[i], y[j], t[k], mu);
            }
        }
    }

    return result;
}

QVector<QVector<QVector<double> > > Parabolic2D(
        QVector<double> &x, QVector<double> &y, QVector<double> &t,
        double h_x, double h_y, double tau, double a, double b, double mu, Method method) {
    int Nx = x.size();
    int Ny = y.size();
    int Nk = t.size();
    double cur_tau;

    if (method == VariableDirectionMethod) {
        cur_tau = tau / 2.0;
    } else if (method == FractionalStepMethod) {
        cur_tau = tau;
    }

    QVector<QVector<QVector<double> > > result(Nk, QVector< QVector<double> >(
                                                   Nx, QVector<double>(Ny, 0)));

    for (int k = 1; k < Nk; ++k) {
        for (int j = 1; j < Ny - 1; ++j) {
            QVector< QVector<double> > matrixA(Nx, QVector<double>(Nx, 0));
            QVector<double> vectorB(Nx, 0);
            double coeff = a * cur_tau / (h_x * h_x);

            for (int i = 1; i < Nx - 1; ++i) {
                matrixA[i][i - 1] = coeff;
                matrixA[i][i] = -(2 * coeff + 1);
                matrixA[i][i + 1] = coeff;

                vectorB[i] = -result[k - 1][i][j];

                if (method == VariableDirectionMethod) {
                    vectorB[i] -= b * cur_tau / (h_y * h_y) *
                            (result[k - 1][i][j - 1] -
                             2 * result[k - 1][i][j] +
                             result[k - 1][i][j + 1])
                            + cur_tau * func(x[i], y[j], t[k - 1] + cur_tau, a, b, mu);
                } else if (method == FractionalStepMethod) {
                    vectorB[i] -= cur_tau / 2.0 * func(x[i], y[j], t[k - 1], a, b, mu);
                }
            }

            matrixA[0][0] = 1;
            matrixA[0][1] = 0;
            matrixA[Nx - 1][Nx - 2] = -1;
            matrixA[Nx - 1][Nx - 1] = 1;

            vectorB[0] = 0;
            vectorB[Nx - 1] = -h_x * std::sin(y[j]) * std::sin(mu * (t[k - 1] + cur_tau));

            QVector<double> res = Sweep(matrixA, vectorB);
            for (int i = 0; i < Nx; ++i) {
                result[k][i][j] = res[i];
            }
        }

        QVector< QVector<double> > tmpResult(Nx, QVector<double>(Ny, 0));
        for (int i = 1; i < Nx - 1; ++i) {
            QVector< QVector<double> > matrixA(Ny, QVector<double>(Ny, 0));
            QVector<double> vectorB(Ny, 0);
            double coeff = b * cur_tau / (h_y * h_y);

            for (int j = 1; j < Ny - 1; ++j) {
                matrixA[j][j - 1] = coeff;
                matrixA[j][j] = -(2 * coeff + 1);
                matrixA[j][j + 1] = coeff;

                vectorB[j] = -result[k][i][j];

                if (method == VariableDirectionMethod) {
                    vectorB[j] -= a * cur_tau / (h_x * h_x) *
                            (result[k][i -1][j] -
                             2 * result[k][i][j] +
                             result[k][i + 1][j])
                            + cur_tau * func(x[i], y[j], t[k - 1] + cur_tau, a, b, mu);
                } else if (method == FractionalStepMethod) {
                    vectorB[j] -= cur_tau / 2.0 * func(x[i], y[j], t[k], a, b, mu);
                }
            }

            matrixA[0][0] = 1;
            matrixA[0][1] = 0;
            matrixA[Ny - 1][Ny - 2] = -1;
            matrixA[Ny - 1][Ny - 1] = 1;

            vectorB[0] = 0;
            vectorB[Ny - 1] = -h_y * std::sin(x[i]) * std::sin(mu * (t[k - 1] + cur_tau));

            QVector<double> res = Sweep(matrixA, vectorB);
            tmpResult[i] = res;
        }

        result[k] = tmpResult;
        for (int j = 0; j < Ny; ++j) {
            result[k][0][j] = 0;
            result[k][Nx - 1][j] = result[k][Nx - 2][j] - h_x * std::sin(y[j]) * std::sin(mu * t[k]);
        }
    }

    return result;
}

