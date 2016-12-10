#ifndef ESPEAKEDIT_H
#define ESPEAKEDIT_H

#include <QWidget>
namespace Ui {
class ESpeakEdit;
}

class ESpeakEdit : public QWidget
{
    Q_OBJECT
public:
    int currentESpeakIndex;
    explicit ESpeakEdit(QWidget *parent = 0);
    ~ESpeakEdit();
    void updateData();

private slots:
    void slot_delete_btn();
    void slot_play_btn();

    void on_add_btn_clicked();

    void on_ok_btn_clicked();



private:
    Ui::ESpeakEdit *ui;
};

#endif // ESPEAKEDIT_H
