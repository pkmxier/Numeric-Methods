#include "widget6.h"
#include "methods.h"

void Widget6::ConfigureLayouts() {
    mainLayout = new QVBoxLayout(this);
    dimensionLayout = new QHBoxLayout(this);
    sourceLayout = new QHBoxLayout(this);
    functionLayout = new QVBoxLayout(this);
    controlsLayout = new QVBoxLayout(this);
    methodsLayout = new QGridLayout(this);
    borderApproxLayout = new QGridLayout(this);
    timeApproxLayout = new QGridLayout(this);

    this->setLayout(mainLayout);
    mainLayout->addLayout(dimensionLayout);
    mainLayout->addLayout(functionLayout);
    mainLayout->addLayout(sourceLayout);

    stepLineEdit[0]->setFixedWidth(50);
    stepLineEdit[1]->setFixedWidth(50);

    maxTLineEdit->setFixedWidth(50);

    dimensionLayout->insertWidget(1, stepLabel[0]);
    dimensionLayout->insertWidget(2, stepLineEdit[0]);
    dimensionLayout->insertWidget(3, stepLabel[1]);
    dimensionLayout->insertWidget(4, stepLineEdit[1]);
    dimensionLayout->insertWidget(5, maxTLabel);
    dimensionLayout->insertWidget(6, maxTLineEdit);
    dimensionLayout->insertWidget(7, sigmaLabel);
    dimensionLayout->addStretch(1);

    functionLayout->insertWidget(1, functionLineEdit);
    functionLayout->insertWidget(2, conditionsLineEdit[0]);
    functionLayout->insertWidget(3, conditionsLineEdit[1]);
    functionLayout->insertWidget(4, initCondLineEdit[0]);
    functionLayout->insertWidget(5, initCondLineEdit[1]);
    functionLayout->insertWidget(6, analyticSolution);
    functionLayout->addStretch(1);

    methodsBox->setFixedHeight(100);
    methodsBox->setLayout(methodsLayout);

    borderApproxBox->setFixedHeight(100);
    borderApproxBox->setLayout(borderApproxLayout);

    timeApproxBox->setFixedHeight(100);
    timeApproxBox->setLayout(timeApproxLayout);

    sourceLayout->addWidget(methodsBox);
    methodsLayout->addWidget(method[0], 0, 0);
    methodsLayout->addWidget(method[1], 1, 0);
    methodsLayout->addWidget(method[2], 2, 0);

    sourceLayout->addWidget(borderApproxBox);
    borderApproxLayout->addWidget(approx[0], 0, 0);
    borderApproxLayout->addWidget(approx[1], 1, 0);
    borderApproxLayout->addWidget(approx[2], 2, 0);

    sourceLayout->addWidget(timeApproxBox);
    timeApproxLayout->addWidget(approx[3], 0, 0);
    timeApproxLayout->addWidget(approx[4], 1, 0);

    method[currentMethod - 1]->setChecked(true);
    approx[currentBorderApprox - 1]->setChecked(true);
    approx[2 + currentTimeApprox]->setChecked(true);

    sourceLayout->addLayout(controlsLayout);

    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loadButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    restoreButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlsLayout->addWidget(runButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(restoreButton);
}

double Widget6::CalcSigma() {
    double tao =
           maxTLineEdit->text().toDouble() / stepLineEdit[1]->text().toDouble();
    double h = 3.14 / stepLineEdit[0]->text().toDouble();
    return tao * tao / (h * h);
}

Widget6::Widget6(QWidget *parent) : QWidget(parent) {
    this->resize(300, 200);
    QString fun = "d^2u/(dt)^2 + 3du/dt ="
                  "d^2u/(dx)^2 + du/dx - u - cosx * exp(-t)";
    functionLineEdit = new QLineEdit(fun);

    stepLabel.resize(2);
    stepLineEdit.resize(2);
    stepLabel[0] = new QLabel("N:");
    stepLineEdit[0] = new QLineEdit("10");
    stepLabel[1] = new QLabel("K:");
    stepLineEdit[1] = new QLineEdit("100");

    maxTLabel = new QLabel("max t:");
    maxTLineEdit = new QLineEdit("3");

    sigmaLabel = new QLabel("sigma: " + QString::number(CalcSigma()));

    conditionsLineEdit.resize(2);
    conditionsLineEdit[0] = new QLineEdit("u'_x(0, t) = exp(-t)");
    conditionsLineEdit[1] = new QLineEdit("u'_x(pi, t) = -exp(-t)");

    initCondLineEdit.resize(2);
    initCondLineEdit[0] = new QLineEdit("u(x, 0) = sin(x)");
    initCondLineEdit[1] = new QLineEdit("u'_t(x, 0) = -sin(x)");

    analyticSolution = new QLineEdit("u(x,t) = exp(-t) * sin(x)");

    methodsBox = new QGroupBox("Методы");
    method.resize(3);
    method[0] = new QRadioButton("Явный");
    method[1] = new QRadioButton("Неявный");

    borderApproxBox = new QGroupBox("Аппроксимация на границе");
    approx.resize(5);
    approx[0] = new QRadioButton("1 порядок, 2 точки");
    approx[1] = new QRadioButton("2 порядок, 3 точки");
    approx[2] = new QRadioButton("2 порядок, 2 точки");

    timeApproxBox = new QGroupBox("Аппроксимация по времени");
    approx[3] = new QRadioButton("1 порядок");
    approx[4] = new QRadioButton("2 порядок");

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
        currentBorderApprox = 1;
    });
    QObject::connect(approx[1], &QRadioButton::clicked, this, [&]() {
        currentBorderApprox = 2;
    });
    QObject::connect(approx[2], &QRadioButton::clicked, this, [&]() {
        currentBorderApprox = 3;
    });

    QObject::connect(approx[3], &QRadioButton::clicked, this, [&]() {
        currentTimeApprox = 1;
    });
    QObject::connect(approx[4], &QRadioButton::clicked, this, [&]() {
        currentTimeApprox = 2;
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

        for (int i = 0; i < x.size(); ++i) {
            x[i] = h * i;
        }

        for (int k = 0; k < t.size(); ++k) {
            t[k] = tao * k;
        }

        QVector< QVector<double> > u;

        if (currentMethod == 1) {
            u = ExplicitHyperbolic(tao, h, x, t, currentBorderApprox,
                                                 currentTimeApprox);
        } else if (currentMethod == 2) {
            u = ImplicitHyperbolic(tao, h, x, t, currentBorderApprox,
                                                 currentTimeApprox);
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

void Widget6::LoadFromFile(const QString &fileName) {

}

