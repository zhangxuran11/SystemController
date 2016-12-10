#ifndef SOFT_KEYBOARD_LOWERCASE_LETTER_H
#define SOFT_KEYBOARD_LOWERCASE_LETTER_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextCodec>

class Soft_Keyboard_Lowercase_Letter : public QWidget
{
    Q_OBJECT
public:
    explicit Soft_Keyboard_Lowercase_Letter(QWidget *parent = 0);
    ~Soft_Keyboard_Lowercase_Letter();

    QPushButton *pushBtn_Lowercase_Letter[26];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;

signals:

public slots:
};

#endif // SOFT_KEYBOARD_LOWERCASE_LETTER_H
