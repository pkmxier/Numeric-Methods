#include "widget5.h"
#include "methods.h"

void Widget5::ConfigureLayouts() {
    mainLayout = new QVBoxLayout(this);
    dimensionLayout = new QHBoxLayout(this);
    sourceLayout = new QHBoxLayout(this);
    functionLayout = new QVBoxLayout(this);
    controlsLayout = new QVBoxLayout(this);
    methodsLayout = new QGridLayout(this);
    approxLayout = new QGridLayout(this);

    this->setLayout(mainLayout);
    mainLayout->addLayout(dimensionLayout);
    mainLayout->addLayout(functionLayout);
    mainLayout->addLayout(sourceLayout);

    stepLineEdit[0]->setFixedWidth(50);
    stepLineEdit[1]->setFixedWidth(50);

    thetaLineEdit->setFixedWidth(50);

    maxTLineEdit->setFixedWidth(50);

    dimensionLayout->insertWidget(1, stepLabel[0]);
    dimensionLayout->insertWidget(2, stepLineEdit[0]);
    dimensionLayout->insertWidget(3, stepLabel[1]);
    dimensionLayout->insertWidget(4, stepLineEdit[1]);
    dimensionLayout->insertWidget(5, thetaLabel);
    dimensionLayout->insertWidget(6, thetaLineEdit);
    dimensionLayout->insertWidget(7, maxTLabel);
    dimensionLayout->insertWidget(8, maxTLineEdit);
    dimensionLayout->insertWidget(9, sigmaLabel);
    dimensionLayout->addStretch(1);

    functionLayout->insertWidget(1, functionLineEdit);
    functionLayout->insertWidget(2, conditionsLineEdit[0]);
    functionLayout->insertWidget(3, conditionsLineEdit[1]);
    functionLayout->insertWidget(4, initCondLineEdit);
    functionLayout->insertWidget(5, analyticSolution);
    functionLayout->addStretch(1);

    methodsBox->setFixedHeight(100);
    methodsBox->setLayout(methodsLayout);

    borderApproxBox->setFixedHeight(100);
    borderApproxBox->setLayout(approxLayout);

    sourceLayout->addWidget(methodsBox);
    methodsLayout->addWidget(method[0], 0, 0);
    methodsLayout->addWidget(method[1], 1, 0);
    methodsLayout->addWidget(method[2], 2, 0);

    sourceLayout->addWidget(borderApproxBox);
    approxLayout->addWidget(approx[0], 0, 0);
    approxLayout->addWidget(approx[1], 1, 0);
    approxLayout->addWidget(approx[2], 2, 0);

    method[currentMethod - 1]->setChecked(true);
    approx[currentApprox - 1]->setChecked(true);

    sourceLayout->addLayout(controlsLayout);

    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loadButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    restoreButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlsLayout->addWidget(runButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(restoreButton);
}

double Widget5::CalcSigma() {
    double tao =
           maxTLineEdit->text().toDouble() / stepLineEdit[1]->text().toDouble();
    double h = 3.14 / stepLineEdit[0]->text().toDouble();
    return tao / (h * h);
}

Widget5::Widget5(QWidget *parent) : QWidget(parent) {
    this->resize(300, 200);
    QString fun = "du/dt = d^2u/(dx)^2 + 0.5exp(-0.5t) * sin(x)";
    functionLineEdit = new QLineEdit(fun);

    stepLabel.resize(2);
    stepLineEdit.resize(2);
    stepLabel[0] = new QLabel("N:");
    stepLineEdit[0] = new QLineEdit("10");
    stepLabel[1] = new QLabel("K:");
    stepLineEdit[1] = new QLineEdit("100");

    thetaLabel = new QLabel(QString::fromWCharArray(L"\u03B8") + ":");
    thetaLineEdit = new QLineEdit("0.5");

    maxTLabel = new QLabel("max t:");
    maxTLineEdit = new QLineEdit("3");

    sigmaLabel = new QLabel("sigma: " + QString::number(CalcSigma()));

    conditionsLineEdit.resize(2);
    conditionsLineEdit[0] = new QLineEdit("u'_x(0, t) = exp(-0.5t)");
    conditionsLineEdit[1] = new QLineEdit("u'_x(pi, t) = -exp(-0.5t)");

    initCondLineEdit = new QLineEdit("u(x, 0) = sin(x)");

    analyticSolution = new QLineEdit("u(x,t) = exp(-0.5t) * sin(x)");

    methodsBox = new QGroupBox("Методы");
    method.resize(3);
    method[0] = new QRadioButton("Явный");
    method[1] = new QRadioButton("Неявный");
    method[2] = new QRadioButton("Кранк-Никлсон");

    borderApproxBox = new QGroupBox("Аппроксимация производной");
    approx.resize(3);
    approx[0] = new QRadioButton("1 порядок, 2 точки");
    approx[1] = new QRadioButton("2 порядок, 3 точки");
    approx[2] = new QRadioButton("2 порядок, 2 точки");

    runButton = new QPushButton("Решить");
    loadButton = new QPushButton("Открыть");
    restoreButton = new QPushButton("Сбросить");

    ConfigureLayouts();

    QObject::connect(method[0], &QRadioButton::clicked, this, [&]() {
        currentMethod = 1;
    });
    QObject::connect(method[1], &QRadioButton::clicked, this, [&]() {
        currentMethod = 2;
    });
    QObject::connect(method[2], &QRadioButton::clicked, this, [&]() {
        currentMethod = 3;
    });

    QObject::connect(approx[0], &QRadioButton::clicked, this, [&]() {
        currentApprox = 1;
    });
    QObject::connect(approx[1], &QRadioButton::clicked, this, [&]() {
        currentApprox = 2;
    });
    QObject::connect(approx[2], &QRadioButton::clicked, this, [&]() {
        currentApprox = 3;
    });

    QObject::connect(stepLineEdit[0], &QLineEdit::textChanged, this, [&]() {
        sigmaLabel->setText("sigma: " + QString::number(CalcSigma()));
    });

    QObject::connect(stepLineEdit[1], &QLineEdit::textChanged, this, [&]() {
        sigmaLabel->setText("sigma: " + QString::number(CalcSigma()));
    });

    QObject::connect(maxTLineEdit, &QLineEdit::textChanged, this, [&]() {
        sigmaLabel->setText("sigma: " + QString::number(CalcSigma()));
    });

    QObject::connect(runButton, &QPushButton::clicked, this, [&]() {
        int Nx = stepLineEdit[0]->text().toInt();
        int Nk = stepLineEdit[1]->text().toInt();
        QVector<double> x(Nx + 1);
        QVector<double> t(Nk + 1);
        double max_t = maxTLineEdit->text().toDouble();
        double tao = max_t / Nk;
        double h = PI / Nx;
        double theta = thetaLineEdit->text().toDouble();

        for (int i = 0; i < x.size(); ++i) {
            x[i] = h * i;
        }

        for (int k = 0; k < t.size(); ++k) {
            t[k] = tao * k;
        }

        QVector< QVector<double> > u;

        if (currentMethod == 1) {
            u = ExplicitParabolic(tao, h, x, t, currentApprox);
        } else if (currentMethod == 2) {
            u = ImplicitParabolic(tao, h, x, t, currentApprox, 1);
        } else if (currentMethod == 3) {
            u = ImplicitParabolic(tao, h, x, t, currentApprox, theta);
        }

        QVector< QVector<double> > analytic_u = AnalyticSolution(x, t);

        double error = 0;
        for (int k = 0; k < u.size(); ++k) {
            for (int i = 0; i < u[k].size(); ++i) {
                error = std::max(error, std::abs(u[k][i] - analytic_u[k][i]));
            }
        }

        qDebug() << "Error: " << error;

        QFile file("analytic");
        QFile file1("method");
        bool is_open = file.open(QIODevice::WriteOnly |
                                 QIODevice::Text);
        bool is_open1 = file1.open(QIODevice::WriteOnly |
                                   QIODevice::Text);
        if (is_open && is_open1) {
            QTextStream stream(&file);
            QTextStream stream1(&file1);

            stream << max_t << endl;
            stream << tao << " " << h << endl;
            stream << analytic_u.size() << " " << analytic_u[0].size() << endl;
            for (int i = 0; i < u.size(); ++i) {
                for (int j = 0; j < u[i].size(); ++j) {
                    stream << analytic_u[i][j] << " ";
                    stream1 << u[i][j] << " ";
                }
                stream << endl;
                stream1 << endl;
            }
        }

        QProcess p;
        QStringList params;

        params << "../graph.py" << "analytic" << "method";
        p.startDetached("python3", params);
        p.waitForFinished(-1);
    });

    QObject::connect(restoreButton, &QPushButton::clicked, this, [&]() {
        functionLineEdit->setText("");
        stepLineEdit[0]->setText("");
        stepLineEdit[1]->setText("");
        for (int i = 0; i < conditionsLineEdit.size(); ++i) {
            conditionsLineEdit[i]->setText("");
        }
    });

    QObject::connect(loadButton, &QPushButton::clicked, this, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("Загрузить"),
                QDir::currentPath() + "/../nm_lab",
                tr("Таблица (*.csv);;Все файлы (*)"));
        LoadFromFile(fileName);
    });
}

void Widget5::LoadFromFile(const QString &fileName) {

}

