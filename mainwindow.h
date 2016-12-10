#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include<QMap>
#include<QLineEdit>
#include"ui_mainwindow.h"
#include"ztpmanager.h"
#include"iodevicelistener.h"
#include"espeakedit.h"
//namespace Ui {
//class MainWindow;
//}
class WorkThread;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class TrainLinePanel;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    IODeviceListener* ioDevice;
    WorkThread* workThread;
    TrainLinePanel* trainLinePanel;
    ESpeakEdit* espeakPanel;

//    NetListener *listener;
//    QTimer flush_ui_timer;
//    NetThread netThread;
//    QMap<QString, QLineEdit**> carLineEditMap;
//    DeviceListener devicelistener;

public:
    explicit MainWindow(QWidget *parent = 0);
    
//private:
    Ui::MainWindow *ui;
private slots:
    void loadLineInfo(int lineId);
    void on_pushButton_clicked();
    void on_espeekButton_clicked();
};

#endif // MAINWINDOW_H
