#ifndef WIDGET7_H
#define WIDGET7_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QGroupBox>
#include <QLabel>
#include <QHeaderView>
#include <QRadioButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpinBox>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <QScrollBar>
#include <QStringList>
#include <QFileDialog>
#include <iostream>
#include <iomanip>
#include <QCloseEvent>
#include <QGraphicsView>
#include <complex>
#include <QProcess>
#include "methods.h"

class Widget8 : public QWidget {
    Q_OBJECT

public:
    explicit Widget8(QWidget *parent = nullptr);

private:
    void LoadFromFile(const QString &fileName);
    void SetAnswer(QString s);

    void ConfigureLayouts();

    QVBoxLayout *mainLayout;
    QHBoxLayout *dimensionLayout;
    QHBoxLayout *sourceLayout;
    QVBoxLayout *functionLayout;
    QGridLayout *methodsLayout;
    QVBoxLayout *controlsLayout;

    QVector<QLineEdit *> conditionsLineEdit;

    QString solution;

    QVector<QLabel *> stepLabel;
    QVector<QLineEdit *> stepLineEdit;

    QVector<QLabel *> paramsLabel;
    QVector<QLineEdit *> paramsLineEdit;

    QLineEdit *functionLineEdit;
    QLineEdit *analyticSolution;

    QGroupBox *methodsBox;
    QVector<QRadioButton *> method;

    QPushButton *runButton,
                *loadButton,
                *restoreButton;

    Method currentMethod = VariableDirectionMethod;
};

#endif // WIDGET7_H
