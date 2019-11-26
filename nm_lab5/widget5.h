#ifndef WIDGET5_H
#define WIDGET5_H
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

class Widget5 : public QWidget {
    Q_OBJECT

public:
    explicit Widget5(QWidget *parent = nullptr);

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
    QGridLayout *approxLayout;
    QVBoxLayout *controlsLayout;

    QVector<QLineEdit *> conditionsLineEdit;
    QLineEdit *initCondLineEdit;

    QString solution;

    QVector<QLabel *> stepLabel;
    QVector<QLineEdit *> stepLineEdit;

    QLabel *thetaLabel;
    QLineEdit *thetaLineEdit;

    QLabel *maxTLabel;
    QLineEdit *maxTLineEdit;

    QLabel *sigmaLabel;

    QLineEdit *functionLineEdit;
    QLineEdit *analyticSolution;

    QGroupBox *methodsBox;
    QGroupBox *borderApproxBox;
    QVector<QRadioButton *> method;
    QVector<QRadioButton *> approx;

    QPushButton *runButton,
                *loadButton,
                *restoreButton;

    int currentMethod = 1;
    int currentApprox = 1;
};

#endif // WIDGET5_H
