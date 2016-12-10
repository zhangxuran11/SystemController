#ifndef KLINEEDIT_H
#define KLINEEDIT_H

#include <QLineEdit>
class KLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit KLineEdit(QWidget *parent = 0);
    
signals:
    void clicked();
protected:
    void mousePressEvent ( QMouseEvent * );
public slots:

};

#endif // KLINEEDIT_H
