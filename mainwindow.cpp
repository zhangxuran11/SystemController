#include<QtGui>
#include "trainlinepanel.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalinfo.h"
#include <QSettings>
#include<QTimer>
#include<QFile>
#include<QDir>
#include<QDebug>
#include<QFileInfo>
#include"workthread.h"
#include"globalinfo_t.h"
#include"carinfo.h"
#include"versionsender.h"
#include<QTime>
#include"ztools.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    new VersionSender("SystemController",1,1,12,ZTools::getCarID());
   ioDevice = new IODeviceListener;
    trainLinePanel = new TrainLinePanel;
    connect(trainLinePanel,SIGNAL(changeCurrentLine(int)),this,SLOT(loadLineInfo(int)));
    espeakPanel = new ESpeakEdit;
    int trainId = GlobalInfo_t::getCurrentTrainIdFromLocalFile();
    loadLineInfo( trainId);
    GlobalInfo::getInstance()->mainWin = this;
    setWindowFlags(Qt::FramelessWindowHint|(windowFlags() & (~Qt::WindowCloseButtonHint)));
    ui->setupUi(this);
    workThread = new WorkThread;
    workThread->start();

    CarInfo::init();

}
void MainWindow::loadLineInfo(int trainId)
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry loadLineInfo");
    ////GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),QString(__func__)+":"+__LINE__);
    GlobalInfo::getInstance()->train_id = trainId;
    QString sql = QString("select * from tb_station_info where train_id=%1 order by indexNu").arg(trainId);
    GlobalInfo::getInstance()->lineInfoSet = GlobalInfo_t::getInstance()->db->query(sql);
    if(GlobalInfo::getInstance()->lineInfoSet.count() == 0)
    {
        qDebug("not found valid data.");
    }
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave loadLineInfo");
}
void MainWindow::on_pushButton_clicked()
{
     //trainLinePanel = new TrainLinePanel;

    //hide();
  //  GlobalInfo::getInstance()->workThread->comSoftDog->pause();
    //QProcess::execute("/appbin/TrainLineEdit");

    trainLinePanel->show();

//    GlobalInfo::getInstance()->workThread->comSoftDog->go();
    //show();
    //system("/appbin/TrainLineEdit");
}

void MainWindow::on_espeekButton_clicked()
{
    espeakPanel->show();
}
