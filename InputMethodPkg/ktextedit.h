#ifndef KTEXTEDIT_H
#define KTEXTEDIT_H

#include <QTextEdit>

class KTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit KTextEdit(QWidget *parent = 0);
    
signals:
    void clicked();
protected:
    void mousePressEvent ( QMouseEvent * );
    
public slots:
    
};

#endif // KTEXTEDIT_H
