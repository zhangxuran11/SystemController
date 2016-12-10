#include "ktextedit.h"

KTextEdit::KTextEdit(QWidget *parent) :
    QTextEdit(parent)
{
}
void KTextEdit::mousePressEvent ( QMouseEvent * )
{
    emit clicked();
}
