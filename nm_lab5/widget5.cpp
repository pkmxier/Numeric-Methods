#include "widget5.h"
#include "methods.h"

void Widget5::ConfigureLayouts() {
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
    dimensionLayout->addStretch(1);

    functionLayout->insertWidget(1, functionLineEdit);
    functionLayout->insertWidget(2, conditionsLineEdit[0]);
    functionLayout->insertWidget(3, conditionsLineEdit[1]);
    functionLayout->insertWidget(4, initCondLineEdit);
    functionLayout->insertWidget(5, analyticSolution);
    functionLayout->addStretch(1);

    methodsBox->setFixedHeight(100);
    methodsBox->setLayout(methodsLayout);

    sourceLayout->addWidget(methodsBox);
    methodsLayout->addWidget(method[0], 0, 0);
    methodsLayout->addWidget(method[1], 1, 0);
    methodsLayout->addWidget(method[2], 2, 0);

    method[0]->setChecked(true);

    sourceLayout->addLayout(controlsLayout);

    runButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    loadButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    restoreButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlsLayout->addWidget(runButton);
    controlsLayout->addWidget(loadButton);
    controlsLayout->addWidget(restoreButton);
}

Widget5::Widget5(QWidget *parent) : QWidget(parent) {
    this->resize(300, 200);
    QString fun = "du/dt = d^2u/(dx)^2 + 0.5exp(-0.5t) * cos(x)";
    functionLineEdit = new QLineEdit(fun);

    stepLabel.resize(2);
    stepLineEdit.resize(2);
    stepLabel[0] = new QLabel("N:");
    stepLineEdit[0] = new QLineEdit("10");
    stepLabel[1] = new QLabel("K:");
    stepLineEdit[1] = new QLineEdit("100");

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

    QObject::connect(runButton, &QPushButton::clicked, this, [&]() {
        int Nx = stepLineEdit[0]->text().toInt();
        int Nk = stepLineEdit[1]->text().toInt();
        QVector<double> x(Nx + 1);
        QVector<double> t(Nk + 1);
        double max_t = 1;
        double tao = max_t / t.size();
        double h = PI / x.size();

        for (int i = 0; i < x.size(); ++i) {
            x[i] = h * i;
        }

        for (int k = 0; k < t.size(); ++k) {
            t[k] = tao * k;
        }

        QVector< QVector<double> > u = ExplicitParabolic(tao, h, x, t);
        QVector< QVector<double> > analytic_u = AnalyticSolution(x, t);

        for (int k = 0; k < u.size(); ++k) {
            for (int i = 0; i < u[k].size(); ++i) {
                std::cout << u[k][i] - analytic_u[k][i] << " ";
            }
            std::cout << std::endl;
        }

        QFile file("analytic");
        QFile file1("method");
        bool is_open = file.open(QIODevice::WriteOnly |
                                 QIODevice::Append |
                                 QIODevice::Text);
        bool is_open1 = file1.open(QIODevice::WriteOnly |
                                   QIODevice::Append |
                                   QIODevice::Text);
        if (is_open && is_open1) {
            QTextStream stream(&file);
            QTextStream stream1(&file1);

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

        params << "../nm_lab5/graph.py" << "analytic" << "method";
        p.start("python3", params);
        p.waitForFinished(-1);
        file.remove();
        file1.remove();
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

