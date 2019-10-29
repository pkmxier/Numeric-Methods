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
    return std::exp(-t) * std::sin(x);
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

QVector<QVector<double> > ExplicitHyperbolic(double tao, double h,
                                             QVector<double> &x,
                                             QVector<double> &t,
                                             int currentBorderApprox,
                                             int currentTimeApprox) {
    int Nx = x.size();
    int Nk = t.size();
    double sigma = tao * tao / (h * h);

    QVector< QVector<double> > result(Nk, QVector<double>(Nx, 0));

    for (int i = 0; i < Nx; ++i) {
        result[0][i] = std::sin(x[i]);
    }

    if (currentTimeApprox == 1) {
        for (int i = 0; i < Nx; ++i) {
            result[1][i] = result[0][i] - tao * std::sin(x[i]);
        }
    } else if (currentTimeApprox == 2) {
        for (int i = 0; i < Nx; ++i) {
            result[1][i] = result[0][i] * (1 - tao * tao / 2)
                - tao * std::sin(x[i])
                + tao * tao / 2 * (2 * std::sin(x[i]) + std::cos(x[i])
                               - std::cos(x[i]) * std::exp(-t[0]));
        }
    }

    for (int k = 2; k < Nk; ++k) {
        for (int i = 1; i < Nx - 1; ++i) {
            result[k][i] = (result[k - 1][i + 1] * sigma * (1 + h / 2)
             + result[k - 1][i] * (2 - 2 * sigma - tao * tao)
             + result[k - 1][i - 1] * sigma * (1 - h / 2)
             + result[k - 2][i] * (1.5 * tao - 1)
             - tao * tao * std::cos(x[i]) * std::exp(-t[k - 1]))
                    / (1 + 1.5 * tao);
        }

        if (currentBorderApprox == 1) {
            result[k][0] = result[k][1] - h * std::exp(-t[k]);
            result[k][Nx - 1] = result[k][Nx - 2]  - h * std::exp(-t[k]);
        } else if (currentBorderApprox == 2) {
            result[k][0] = (- 2 * h * std::exp(-t[k])
                            + 4 * result[k][1]
                            - result[k][2]) / 3;
            result[k][Nx - 1] = (- 2 * h * std::exp(-t[k])
                                 + 4 * result[k][Nx - 2]
                                 - result[k][Nx - 3]) / 3;
        } else if (currentBorderApprox == 3) {
            result[k][0] = (result[k][1] - h * std::exp(-t[k])
                    + result[k - 1][0] * (1 / sigma + 1.5 * tao / sigma)
                    - result[k - 2][0] * 1 / (2 * sigma)
                    + h * h / 2 * std::exp(-t[k])
                    - h * h / 2 * std::cos(x[0]) * std::exp(-t[k]))
                    / (1 + 1 / (2 * sigma) + 1.5 * tao / sigma + h * h / 2);
            result[k][Nx - 1] = (result[k][Nx - 2] - h * std::exp(-t[k])
                    + result[k - 1][Nx - 1] * (1 / sigma + 1.5 * tao / sigma)
                    - result[k - 2][Nx - 1] * 1 / (2 * sigma)
                    - h * h / 2 * std::exp(-t[k])
                    - h * h / 2 * std::cos(x[Nx - 1]) * std::exp(-t[k]))
                    / (1 + 1 / (2 * sigma) + 1.5 * tao / sigma + h * h / 2);
        }
    }

    return result;
}

QVector<QVector<double> > ImplicitHyperbolic(double tao, double h,
                                             QVector<double> &x,
                                             QVector<double> &t,
                                             int currentBorderApprox,
                                             int currentTimeApprox) {
    int Nx = x.size();
    int Nk = t.size();

    QVector< QVector<double> > result(Nk, QVector<double>(Nx, 0));

    for (int i = 0; i < Nx; ++i) {
        result[0][i] = std::sin(x[i]);
    }

    if (currentTimeApprox == 1) {
        for (int i = 0; i < Nx; ++i) {
            result[1][i] = result[0][i] - tao * std::sin(x[i]);
        }
    } else if (currentTimeApprox == 2) {
        for (int i = 0; i < Nx; ++i) {
            result[1][i] = result[0][i] * (1 - tao * tao / 2)
                - tao * std::sin(x[i])
                + tao * tao / 2 * (2 * std::sin(x[i]) + std::cos(x[i])
                               - std::cos(x[i]) * std::exp(-t[0]));
        }
    }

    QVector< QVector<double> > a(Nx, QVector<double>(Nx, 0));
    QVector<double> b(Nx, 0);

    for (int i = 1; i < a.size() - 1; ++i) {
        a[i][i - 1] = 1 / (h * h) - 1 / (2 * h);
        a[i][i] = -(1 / (tao * tao) + 1.5 / tao + 2 / (h * h) + 1);
        a[i][i + 1] = 1 / (h * h) + 1 / (2 * h);
    }

    for (int k = 2; k < Nk; ++k) {
        for (int i = 1; i < a.size() - 1; ++i) {
            b[i] = std::cos(x[i]) * std::exp(-t[k])
                    - result[k - 1][i] * 2 / (tao * tao)
                    - result[k - 2][i] * (1.5 / tao - 1 / (tao * tao));
        }


        if (currentBorderApprox == 1) {
            a[0][0] = 1;
            a[0][1] = -1;
            a[a.size() - 1][a.size() - 2] = 1;
            a[a.size() - 1][a.size() - 1] = -1;

            b[0] = -h * std::exp(-t[k]);
            b[b.size() - 1] = h * std::exp(-t[k]);
        } else if (currentBorderApprox == 2) {
            a[0][0] = -3 + a[1][0] / (1 / (h * h) + 1 / (2 * h));
            a[0][1] = 4 + a[1][1] / (1 / (h * h) + 1 / (2 * h));
            a[a.size() - 1][a.size() - 2] =
               -4 - a[a.size() - 2][a.size() - 2] / (1 / (h * h) - 1 / (2 * h));
            a[a.size() - 1][a.size() - 1] =
                3 - a[a.size() - 2][a.size() - 1] / (1 / (h * h) - 1 / (2 * h));

            b[0] = 2 * h * std::exp(-t[k]) + b[1] / (1 / (h * h) + 1 / (2 * h));
            b[b.size() - 1] = -2 * h * std::exp(-t[k])
                    - b[b.size() - 2] / (1 / (h * h) - 1 / (2 * h));
        } else if (currentBorderApprox == 3) {

        }

        result[k] = Sweep(a, b);
    }

    return result;
}
