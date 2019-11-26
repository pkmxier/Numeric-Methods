#ifndef WIDGET6_H
#define WIDGET6_H
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

class Widget6 : public QWidget {
    Q_OBJECT

public:
    explicit Widget6(QWidget *parent = nullptr);

private:
    void LoadFromFile(const QString &fileName);
    void SetAnswer(QString s);

    void ConfigureLayouts();

    double CalcSigma();

    QVBoxLayout *mainLayout;
    QHBoxLayout *dimensionLayout;
    QHBoxLayout *sourceLayout;
    QVBoxLayout *functionLayout;
    QGridLayout *methodsLayout;
    QGridLayout *borderApproxLayout;
    QGridLayout *timeApproxLayout;
    QVBoxLayout *controlsLayout;

    QVector<QLineEdit *> conditionsLineEdit;
    QVector<QLineEdit *> initCondLineEdit;

    QString solution;

    QVector<QLabel *> stepLabel;
    QVector<QLineEdit *> stepLineEdit;

    QLabel *maxTLabel;
    QLineEdit *maxTLineEdit;

    QLabel *sigmaLabel;

    QLineEdit *functionLineEdit;
    QLineEdit *analyticSolution;

    QGroupBox *methodsBox;
    QGroupBox *borderApproxBox;
    QGroupBox *timeApproxBox;
    QVector<QRadioButton *> method;
    QVector<QRadioButton *> approx;

    QPushButton *runButton,
                *loadButton,
                *restoreButton;

    int currentMethod = 1;
    int currentBorderApprox = 1;
    int currentTimeApprox = 1;
};

#endif // WIDGET6_H
