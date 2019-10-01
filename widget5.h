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

class Widget5 : public QWidget {
    Q_OBJECT

public:
    explicit Widget5(QWidget *parent = nullptr);

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

    QVector<QLineEdit *> limitsLineEdit,
                         conditionsLineEdit;

    QString solution;

    QLabel *limitsLabel,
           *conditionsLabel;

    QVector<QLabel *> stepLabel;
    QVector<QLineEdit *> stepLineEdit;

    QLineEdit *functionLineEdit;

    QGroupBox *methodsBox;
    QVector<QRadioButton *> method;

    QPushButton *runButton,
                *loadButton,
                *restoreButton;

    int currentMethod = 1;
};

#endif // WIDGET5_H
