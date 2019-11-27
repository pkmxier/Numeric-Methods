#include "widget8.h"

void Widget8::ConfigureLayouts() {
    mainLayout = new QVBoxLayout(this);
    dimensionLayout = new QHBoxLayout(this);
    sourceLayout = new QHBoxLayout(this);
    functionLayout = new QVBoxLayout(this);
    controlsLayout = new QVBoxLayout(this);
    methodsLayout = new QGridLayout(this);

    this->setLayout(mainLayout);
    mainLayout->addLayout(dimensionLayout);
    mainLayout->addLayout(functionLayout);
    mainLayout->addLayout(sourceLayout);

    int cnt = 1;
    for (int i = 0; i < stepLineEdit.size(); ++i) {
        stepLineEdit[i]->setFixedWidth(50);

        dimensionLayout->insertWidget(cnt++, stepLabel[i]);
        dimensionLayout->insertWidget(cnt++, stepLineEdit[i]);
    }

    for (int i = 0; i < paramsLineEdit.size(); ++i) {
        paramsLineEdit[i]->setFixedWidth(20);
        dimensionLayout->insertWidget(cnt++, paramsLabel[i]);
        dimensionLayout->insertWidget(cnt++, paramsLineEdit[i]);
    }
    dimensionLayout->addStretch(1);

    functionLayout->insertWidget(1, functionLineEdit);
    cnt = 2;
    for (int i = 0; i < conditionsLineEdit.size(); ++i) {
        functionLayout->insertWidget(cnt++, conditionsLineEdit[i]);
    }
    functionLayout->insertWidget(7, analyticSolution);
    functionLayout->addStretch(1);

    methodsBox->setFixedHeight(100);
    methodsBox->setLayout(methodsLayout);

    sourceLayout->addWidget(methodsBox);
    methodsLayout->addWidget(method[0], 0, 0);
    methodsLayout->addWidget(method[1], 1, 0);

    method[0]->setChecked(true);

    sourceLayout->addLayout(controlsLayout);

    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loadButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    restoreButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlsLayout->addWidget(runButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(restoreButton);
}

Widget8::Widget8(QWidget *parent) : QWidget(parent) {
    this->resize(300, 200);
    QString fun = "du/dt = a * d^2u/(dx)^2 + b * d^2u/(dy)^2 + sin(x) * sin(y) *"
                  "(mu * cos(mu * t) + (a + b) * sin(mu * t))";
    functionLineEdit = new QLineEdit(fun);

    stepLabel.resize(3);
    stepLineEdit.resize(3);
    stepLabel[0] = new QLabel("Nx:");
    stepLineEdit[0] = new QLineEdit("10");
    stepLabel[1] = new QLabel("Ny:");
    stepLineEdit[1] = new QLineEdit("10");
    stepLabel[2] = new QLabel("Nk:");
    stepLineEdit[2] = new QLineEdit("10");

    paramsLabel.resize(4);
    paramsLineEdit.resize(4);
    paramsLabel[0] = new QLabel("max_t");
    paramsLineEdit[0] = new QLineEdit("3");
    paramsLabel[1] = new QLabel("a");
    paramsLineEdit[1] = new QLineEdit("1");
    paramsLabel[2] = new QLabel("b");
    paramsLineEdit[2] = new QLineEdit("1");
    paramsLabel[3] = new QLabel("mu");
    paramsLineEdit[3] = new QLineEdit("1");

    conditionsLineEdit.resize(5);
    conditionsLineEdit[0] = new QLineEdit("u(0, y, t) = 0");
    conditionsLineEdit[1] = new QLineEdit("u'_x(pi, y, t) = -sin(y) * sin(mu * t)");
    conditionsLineEdit[2] = new QLineEdit("u(x, 0, t) = 0");
    conditionsLineEdit[3] = new QLineEdit("u'_y(x, pi, t) = -sin(x) * sin(mu * t)");
    conditionsLineEdit[4] = new QLineEdit("u(x, y, 0) = 0");

    analyticSolution = new QLineEdit("u(x, y, t) = sin(x) * sin(y) * sin(mu * t)");

    methodsBox = new QGroupBox("Методы");
    method.resize(2);
    method[0] = new QRadioButton("Переменных направлений");
    method[1] = new QRadioButton("Дробных шагов");

    runButton = new QPushButton("Решить");
    loadButton = new QPushButton("Открыть");
    restoreButton = new QPushButton("Сбросить");

    ConfigureLayouts();

    QObject::connect(method[0], &QRadioButton::clicked, this, [&] () {
        currentMethod = VariableDirectionMethod;
    });

    QObject::connect(method[1], &QRadioButton::clicked, this, [&] () {
        currentMethod = FractionalStepMethod;
    });

    QObject::connect(runButton, &QPushButton::clicked, this, [&]() {
        int Nx = stepLineEdit[0]->text().toInt();
        int Ny = stepLineEdit[1]->text().toInt();
        int Nk = stepLineEdit[2]->text().toInt();
        QVector<double> x(Nx + 1);
        QVector<double> y(Ny + 1);
        QVector<double> t(Nk + 1);
        double max_t = paramsLineEdit[0]->text().toDouble();
        double a = paramsLineEdit[1]->text().toDouble();
        double b = paramsLineEdit[2]->text().toDouble();
        double mu = paramsLineEdit[3]->text().toDouble();
        double h_x = PI / Nx;
        double h_y = PI / Ny;
        double tau = max_t / Nk;

        for (int i = 0; i < x.size(); ++i) {
            x[i] = h_x * i;
        }

        for (int i = 0; i < y.size(); ++i) {
            y[i] = h_y * i;
        }

        for (int i = 0; i < t.size(); ++i) {
            t[i] = tau * i;
        }

        QVector< QVector< QVector<double> > > u =
                Parabolic2D(x, y, t, h_x, h_y, tau, a, b, mu, currentMethod);
        QVector< QVector< QVector<double> > > analytic_u =
                AnalyticSolution(x, y, t, mu);

        double error = 0;
        for (int k = 0; k < u.size(); ++k) {
//            std::cout << "k: " << k << std::endl;
            for (int i = 0; i < u[k].size(); ++i) {
                for (int j = 0; j < u[k][i].size(); ++j) {
                    error = std::max(error, std::abs(u[k][i][j] - analytic_u[k][i][j]));
//                    std::cout << std::abs(u[k][i][j] - analytic_u[k][i][j]) << " ";
                }
//                std::cout << std::endl;
            }
//            std::cout << std::endl;
        }

        qDebug() << "Error: " << error;

//        QFile file("analytic");
//        QFile file1("method");
//        bool is_open = file.open(QIODevice::WriteOnly |
//                                 QIODevice::Text);
//        bool is_open1 = file1.open(QIODevice::WriteOnly |
//                                   QIODevice::Text);
//        if (is_open && is_open1) {
//            QTextStream stream(&file);
//            QTextStream stream1(&file1);

//            stream << h_x << " " << h_y << endl;
//            stream << analytic_u.size() << " " << analytic_u[0].size() << endl;
//            for (int i = 0; i < u.size(); ++i) {
//                for (int j = 0; j < u[i].size(); ++j) {
//                    stream << analytic_u[i][j] << " ";
//                    stream1 << u[i][j] << " ";
//                }
//                stream << endl;
//                stream1 << endl;
//            }
//        }

//        QProcess p;
//        QStringList params;

//        params << "../graph.py" << "analytic" << "method";
//        p.startDetached("python3", params);
//        p.waitForFinished(-1);
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

void Widget8::LoadFromFile(const QString &fileName) {
}

