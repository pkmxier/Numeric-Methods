#include "widget5.h"
//#include "../nm_lab2/graph_widget.h"
//#include "../nm_lab1/methods.h"

#ifdef _WIN32
const QString PATH = "C:/Users/pkmixer/Documents/nm_lab4/";
#endif

#ifdef linux
const QString PATH = "/home/pkmixer/downloads/Numeric_methods/nm_lab4/";
#endif

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

    for (int i = 0; i < limitsLineEdit.size(); ++i) {
        limitsLineEdit[i] = new QLineEdit("");
    }
    for (int i = 0; i < conditionsLineEdit.size(); ++i) {
        conditionsLineEdit[i] = new QLineEdit("");
    }

    functionLayout->addStretch(1);
    functionLayout->insertWidget(1, functionLineEdit);
    functionLayout->addStretch(1);
    functionLayout->insertWidget(2, conditionsLineEdit[0]);
    functionLayout->addStretch(1);
    functionLayout->insertWidget(3, conditionsLineEdit[1]);
    functionLayout->addStretch(1);

    stepLineEdit[0]->setFixedWidth(50);
    stepLineEdit[1]->setFixedWidth(50);

    dimensionLayout->addStretch(1);
    dimensionLayout->insertWidget(1, stepLabel[0]);
    dimensionLayout->addStretch(1);
    dimensionLayout->insertWidget(2, stepLineEdit[0]);
    dimensionLayout->addStretch(1);
    dimensionLayout->insertWidget(3, stepLabel[1]);
    dimensionLayout->addStretch(1);
    dimensionLayout->insertWidget(4, stepLineEdit[1]);


    dimensionLayout->setAlignment(Qt::AlignTop);

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
    functionLineEdit = new QLineEdit;
    stepLabel.resize(2);
    stepLineEdit.resize(2);
    stepLabel[0] = new QLabel("Шаг по x:");
    stepLineEdit[0] = new QLineEdit;
    stepLabel[1] = new QLabel("Шаг по t:");
    stepLineEdit[1] = new QLineEdit;
    methodsBox = new QGroupBox("Методы");
    limitsLabel = new QLabel("Отрезок:");
    conditionsLabel = new QLabel("Условия:");
    limitsLineEdit.resize(2);
    conditionsLineEdit.resize(2);
    method.resize(3);
    method[0] = new QRadioButton("Явный");
    method[1] = new QRadioButton("Неявный");
    method[2] = new QRadioButton("Кранк-Никлсон");
    runButton = new QPushButton("Решить");
    loadButton = new QPushButton("Открыть");
    restoreButton = new QPushButton("Сбросить");

    ConfigureLayouts();

    LoadFromFile(PATH + "2lab.csv");

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

    });

    QObject::connect(restoreButton, &QPushButton::clicked, this, [&]() {
        functionLineEdit->setText("");
        stepLineEdit[0]->setText("");
        stepLineEdit[1]->setText("");
        for (int i = 0; i < limitsLineEdit.size(); ++i) {
            limitsLineEdit[i]->setText("");
        }
        for (int i = 0; i < conditionsLineEdit.size(); ++i) {
            conditionsLineEdit[i]->setText("");
        }
    });

    QObject::connect(loadButton, &QPushButton::clicked, this, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this,
                tr("Загрузить матрицу из файла"),
                QDir::currentPath() + "/../nm_lab4",
                tr("Таблица (*.csv);;Все файлы (*)"));
        LoadFromFile(fileName);
    });

    emit functionLineEdit->textChanged("");
    emit method[0]->clicked();
}

void Widget5::LoadFromFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QString function(file.readLine().simplified());
    QVector<QString> limits =
            QString(file.readLine().simplified()).split(',').toVector();
    QVector<QString> conditions =
            QString(file.readLine().simplified()).split(',').toVector();
    solution = file.readLine().simplified();
    QString h = file.readLine().simplified();

    functionLineEdit->setText(function);
    for (int i = 0; i < limits.size(); ++i) {
        limitsLineEdit[i]->setText(limits[i]);
    }
    for (int i = 0; i < conditions.size(); ++i) {
        conditionsLineEdit[i]->setText(conditions[i]);
    }
    stepLineEdit[0]->setText(h);
}

