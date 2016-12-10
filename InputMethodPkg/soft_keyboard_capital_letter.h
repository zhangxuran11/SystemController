#ifndef SOFT_KEYBOARD_CAPITAL_LETTER_H
#define SOFT_KEYBOARD_CAPITAL_LETTER_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextCodec>

class Soft_keyboard_Capital_Letter : public QWidget
{
    Q_OBJECT
public:
    explicit Soft_keyboard_Capital_Letter(QWidget *parent = 0);
    ~Soft_keyboard_Capital_Letter();

    QPushButton *pushBtn_Capital_Letter[26];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;

signals:

public slots:
};

#endif // SOFT_KEYBOARD_CAPITAL_LETTER_H
