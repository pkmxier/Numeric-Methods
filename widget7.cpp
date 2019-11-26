#include "widget7.h"
#include "methods.h"

void Widget7::ConfigureLayouts() {
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

    stepLineEdit[0]->setFixedWidth(50);
    stepLineEdit[1]->setFixedWidth(50);

    dimensionLayout->insertWidget(1, stepLabel[0]);
    dimensionLayout->insertWidget(2, stepLineEdit[0]);
    dimensionLayout->insertWidget(3, stepLabel[1]);
    dimensionLayout->insertWidget(4, stepLineEdit[1]);
    dimensionLayout->insertWidget(5, epsilonLabel);
    dimensionLayout->insertWidget(6, epsilonLineEdit);
    dimensionLayout->addStretch(1);

    functionLayout->insertWidget(1, functionLineEdit);
    functionLayout->insertWidget(2, conditionsLineEdit[0]);
    functionLayout->insertWidget(3, conditionsLineEdit[1]);
    functionLayout->insertWidget(4, conditionsLineEdit[2]);
    functionLayout->insertWidget(5, conditionsLineEdit[3]);
    functionLayout->insertWidget(6, analyticSolution);
    functionLayout->addStretch(1);

    methodsBox->setFixedHeight(100);
    methodsBox->setLayout(methodsLayout);

    sourceLayout->addWidget(methodsBox);
    methodsLayout->addWidget(method[0], 0, 0);

    method[currentMethod - 1]->setChecked(true);

    sourceLayout->addLayout(controlsLayout);

    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loadButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    restoreButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlsLayout->addWidget(runButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(restoreButton);
}

Widget7::Widget7(QWidget *parent) : QWidget(parent) {
    this->resize(300, 200);
    QString fun = "d^2u/(dx)^2 + d^2u/(dy)^2 = -u";
    functionLineEdit = new QLineEdit(fun);

    stepLabel.resize(2);
    stepLineEdit.resize(2);
    stepLabel[0] = new QLabel("Nx:");
    stepLineEdit[0] = new QLineEdit("100");
    stepLabel[1] = new QLabel("Ny:");
    stepLineEdit[1] = new QLineEdit("100");

    epsilonLabel = new QLabel("epsilon:");
    epsilonLineEdit = new QLineEdit("0.01");

    conditionsLineEdit.resize(4);
    conditionsLineEdit[0] = new QLineEdit("u'_x(0, y) = cos(y)");
    conditionsLineEdit[1] = new QLineEdit("u'_x(1, y) - u(1, y) = 0");
    conditionsLineEdit[2] = new QLineEdit("u(x, 0) = x");
    conditionsLineEdit[3] = new QLineEdit("u(x, pi/2) = 0");

    analyticSolution = new QLineEdit("u(x,t) = x * cos(y)");

    methodsBox = new QGroupBox("Методы");
    method.resize(3);
    method[0] = new QRadioButton("Явный");
    method[1] = new QRadioButton("Неявный");

    runButton = new QPushButton("Решить");
    loadButton = new QPushButton("Открыть");
    restoreButton = new QPushButton("Сбросить");

    ConfigureLayouts();

    QObject::connect(runButton, &QPushButton::clicked, this, [&]() {
        int Nx = stepLineEdit[0]->text().toInt();
        int Ny = stepLineEdit[1]->text().toInt();
        QVector<double> x(Nx + 1);
        QVector<double> y(Ny + 1);
        double h_x = 1.0 / Nx;
        double h_y = PI / (2.0 * Ny);
        double epsilon = epsilonLineEdit->text().toDouble();

        for (int i = 0; i < x.size(); ++i) {
            x[i] = h_x * i;
        }

        for (int i = 0; i < y.size(); ++i) {
            y[i] = h_y * i;
        }

        QVector< QVector<double> > u;

        if (currentMethod == 1) {
            u = Elliptic(h_x, h_y, x, y, epsilon);
        }

        QVector< QVector<double> > analytic_u = AnalyticSolution(x, y);

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

            stream << h_x << " " << h_y << endl;
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

void Widget7::LoadFromFile(const QString &fileName) {
}

