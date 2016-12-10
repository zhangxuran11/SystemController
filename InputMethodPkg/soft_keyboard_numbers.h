#ifndef SOFT_KEYBOARD_NUMBERS_H
#define SOFT_KEYBOARD_NUMBERS_H

#include <QtGui>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextCodec>

class Soft_Keyboard_Numbers : public QWidget
{
    Q_OBJECT
public:
    explicit Soft_Keyboard_Numbers(QWidget *parent = 0);
    ~Soft_Keyboard_Numbers();

    QPushButton *pushBtn_Numbers[21];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;

    QLineEdit *lineEdit;

signals:

public slots:
};

#endif // SOFT_KEYBOARD_NUMBERS_H
