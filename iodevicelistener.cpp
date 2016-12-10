#include "iodevicelistener.h"
#include "config.h"
#include"globalinfo.h"
#include"ztpmanager.h"
#include <QTimer>
#include <QDebug>
#include<QNetworkInterface>
#include "ztools.h"
IODeviceListener::IODeviceListener(QObject *parent) :
    QObject(parent),cs1(CS1_GPIO,GPIO::H),cs2(CS2_GPIO,GPIO::H),cs3(CS3_GPIO,GPIO::H),
    cs4(CS4_GPIO,GPIO::H),slm1(SLM1_GPIO,GPIO::L),slm2(SLM2_GPIO,GPIO::L)
{
    comZtpManager = new ZTPManager;
    carId = ZTools::getCarID();
    cs_slm_timer_id = startTimer(1000);
    if(cs_slm_timer_id == 0)
        qDebug("Create cs_slm_timer_id failed!!");
    else
        qDebug("Create cs_slm_timer_id success!!");
//    cs_slm_Timer = new QTimer;
//    cs_slm_Timer->setInterval(1000);
//    connect(cs_slm_Timer,SIGNAL(timeout()),this,SLOT(onCsSlmTimeout()));
//    cs_slm_Timer->setSingleShot(false);
//    cs_slm_Timer->start();

}
void IODeviceListener::timerEvent(QTimerEvent* event)
{
   if(event->timerId() == cs_slm_timer_id)
   {
       onCsSlmTimeout();
   }
}
QString IODeviceListener::getCs(int ind)
{
    bool res;
    switch (ind)
    {
    case 1:
        res = cs1.isValid();
        break;
    case 2:
        res = cs2.isValid();
        break;
    case 3:
        res = cs3.isValid();
        break;
    case 4:
        res = cs4.isValid();
        break;
    default:
        res = false;
        break;
    }
    return res?"Y":"N";
}
QString IODeviceListener::getSlm(int ind)
{
    bool res;
    switch (ind)
    {
    case 1:
        res = slm1.isValid();
        break;
    case 2:
        res = slm2.isValid();
        break;
    default:
        res = false;
        break;
    }
    return res?"OPEN":"CLOSE";
}
void IODeviceListener::onCsSlmTimeout()
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry onCsSlmTimeout");
    static int sec = 0;
    if(++sec >= 10)
    {
        qDebug(" onCsSlmTimeout");
        sec = 0;
    }
    QString sta_cs1 = getCs(1);
    QString sta_cs2 = getCs(2);
    QString sta_cs3 = getCs(3);
    QString sta_cs4 = getCs(4);

    QString sta_slm1 = getSlm(1);
    QString sta_slm2 = getSlm(2);
    ZTPprotocol ztp;
    ztp.addPara("T","SLM_STATE");
    ztp.addPara("CAR_ID",QString::number( carId));//车厢号
    ztp.addPara("SLM1",sta_slm1);
    ztp.addPara("SLM2",sta_slm2);
    comZtpManager->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),3320);
    ztp.clear();
    ztp.addPara("T","CS_STATE");
    ztp.addPara("CAR_ID",QString::number( carId));//车厢号
    ztp.addPara("CS1",sta_cs1);
    ztp.addPara("CS2",sta_cs2);
    ztp.addPara("CS3",sta_cs3);
    ztp.addPara("CS4",sta_cs4);
    comZtpManager->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),3321);
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave onCsSlmTimeout");
}
IODeviceListener::~IODeviceListener()
{
 //   delete cs_slm_Timer;
}
