#ifndef SOFT_KEYBOARD_H
#define SOFT_KEYBOARD_H

#include <QtGui>
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QFont>
#include <QtDebug>

#include "soft_keyboard_numbers.h"
#include "soft_keyboard_complete.h"
#include "soft_keyboard_capital_letter.h"
#include "soft_keyboard_lowercase_letter.h"
#include "soft_keyboard_symbol.h"

class Soft_Keyboard : public QDialog
{
    Q_OBJECT
    QModelIndex modelIndex;
    QObject* currentEditWidget;
    QObject* senderWidget;
    QLineEdit* lineEdit;
public:
    Soft_Keyboard(QWidget *parent = 0);
    ~Soft_Keyboard();

    void connect_Letter_PushBtn_Click();

    QTabWidget *tabWidget;
    QPushButton *pushBtn_Clean;
    QPushButton *pushBtn_Space;
    QPushButton *pushBtn_Backspace;
    QPushButton *pushBtn_Quit;

    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;

    Soft_Keyboard_Numbers *soft_Keyboard_Numbers;
    Soft_Keyboard_Complete *soft_Keyboard_Complete;
    Soft_keyboard_Capital_Letter *soft_keyboard_Capital_Letter;
    Soft_Keyboard_Lowercase_Letter *soft_Keyboard_Lowercase_Letter;
    Soft_Keyboard_Symbol *soft_Keyboard_Symbol;

signals:
private slots:
    void slot_Letter_PushBtn_Clicked();
    void slot_Clean_PushBtn_Clicked();
    void slot_Space_PushBtn_Clicked();
    void slot_BackSpace_PusnBtn_Clicked();
    void slot_OK_PusnBtn_Clicked();
public slots:
    void show();
    void show(const QModelIndex&);
};

#endif // SOFT_KEYBOARD_H
