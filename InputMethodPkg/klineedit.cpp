#include "klineedit.h"

KLineEdit::KLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}
void KLineEdit::mousePressEvent ( QMouseEvent * )
{
    emit clicked();
}
