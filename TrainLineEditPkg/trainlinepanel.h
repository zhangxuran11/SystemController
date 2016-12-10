#ifndef TRAINLINEPANEL_H
#define TRAINLINEPANEL_H

#include <QWidget>
#include<QMap>
#include"soft_keyboard.h"
namespace Ui {
class TrainLinePanel;
}

class TrainLinePanel : public QWidget
{
    Q_OBJECT
    bool f_updataDB;
    QMap<QString,QTableWidget*> list_table_map;//待废弃
//    Soft_Keyboard *keyboard;
public:
    explicit TrainLinePanel(QWidget *parent = 0);
    ~TrainLinePanel();
    void show();
    void hide();
signals:
    void changeCurrentLine(int train_id);
private slots:

    void on_add_line_btn_clicked();

    void on_add_line_ok_btn_clicked();

    void on_delete_station_btn_clicked();

    void on_add_station_btn_clicked();

    //void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_ok_btn_clicked();

    void on_setCurLineBtn_clicked();

    void on_runStateBtn_clicked();
    void updateDatabase(QTableWidgetItem* item);


    void clsHideBtnF();
    void on_hideBtn1_clicked();
    void on_hideBtn2_clicked();


    void on_listWidget_currentRowChanged(int currentRow);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    void __initTableHeader();
//    void saveStation();
    void initTable();
    void initLineList();
    void init();
    Ui::TrainLinePanel *ui;
};

#endif // TRAINLINEPANEL_H
