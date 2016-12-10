#ifndef SOFT_KEYBOARD_SYMBOL_H
#define SOFT_KEYBOARD_SYMBOL_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextCodec>

class Soft_Keyboard_Symbol : public QWidget
{
    Q_OBJECT
public:
    explicit Soft_Keyboard_Symbol(QWidget *parent = 0);
    ~Soft_Keyboard_Symbol();

    QPushButton *pushBtn_Symbol[38];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;

signals:

public slots:
};

#endif // SOFT_KEYBOARD_SYMBOL_H
