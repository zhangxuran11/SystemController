#ifndef SOFT_KEYBOARD_COMPLETE_H
#define SOFT_KEYBOARD_COMPLETE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QTextCodec>

class Soft_Keyboard_Complete : public QWidget
{
    Q_OBJECT
public:
    explicit Soft_Keyboard_Complete(QWidget *parent = 0);
    ~Soft_Keyboard_Complete();

    QPushButton *pushBtn_Complete[87];
    QGridLayout *gridLayout;
    QVBoxLayout *vBoxLayout;

signals:

public slots:
};

#endif // SOFT_KEYBOARD_COMPLETE_H
