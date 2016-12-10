#include "workthread.h"
#include"mainwindow.h"
#include"bhctool.h"
#include"globalinfo.h"
#include"globalinfo_t.h"
#include<QTimer>
#include<QTime>
#include<QtCore>
#include<QLabel>
#include"gpsdevice.h"
#include"ztpmanager.h"
#include"gpsmapmanager.h"
#include"config.h"
#include"dbservice.h"
#include"math.h"
#include"ztools.h"
#include"modbusmanager.h"
#include "zdownloadthread.h"
#include"carinfo.h"
#include<QDebug>
static int debug = 0;
static int sendTempTimerId = 0;
static float temp = 0;
static float outTemp = 0;
static void procSlmInfo(const ZTPprotocol& ztp);
WorkThread::WorkThread(QObject *parent) :
    QThread(parent)
{
    comBroadcastManager = NULL;
    GlobalInfo::getInstance()->workThread = this;
    rmc.dateTime.setDate(QDate::currentDate());
    rmc.dateTime.setTime(QTime::currentTime());
    rmc.speed = 0;
    hwCLockId = startTimer(60*60*1000);
    carrierClockId = startTimer(1000);
}
WorkThread::~WorkThread(){
    delete gpsTimer;
}


void WorkThread::onRecvNetMsg()
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry onRecvNetMsg");
//    qDebug("entry file:%s line:%d func:%s thread:%ld",__FILE__,__LINE__,__FUNCTION__,QThread::currentThreadId());
    ZTPprotocol ztp;
    ZTPManager* ztpm = dynamic_cast<ZTPManager*>(sender());
    if(ztpm == NULL)
        return;
    ZTPManager::ResultState state = ztpm->getOneZtp(ztp);
//    ztp.print();
    if(state == ZTPManager::FAILED)
    {
//        qDebug("AAAAAAAAAAAAAAAAAA ");
    }
    if(ztp.getPara("T") == "IPaddress1")
    {
        if(GlobalInfo::getInstance()->VServerIP != ztp.getPara("mainvideoserver"))
            GlobalInfo::getInstance()->VServerIP = ztp.getPara("mainvideoserver");

        if(GlobalInfo::getInstance()->VServerIP != NULL)
            delete ztpm;
    }
    else if(ztp.getPara("T") == "G_INFO")
    {
        if(comBroadcastManager != NULL)
        {
            static unsigned int cnt = 0;
            cnt++;
            comBroadcastManager->dateTime = QDateTime::fromString(ztp.getPara("DateTime"),"yy-MM-dd hh:mm:ss");
            comBroadcastManager->speed = ztp.getPara("Speed").toFloat();
            comBroadcastManager->trainId = ztp.getPara("TrainNum").toInt();
            comBroadcastManager->carId = ZTools::getCarID();
            if(cnt > 10) {
                comBroadcastManager->startStationEN = ztp.getPara("StartStation");
                comBroadcastManager->endStationEN = ztp.getPara("EndStation");
                comBroadcastManager->startStationThai = ztp.getPara("StartStation_th");
                comBroadcastManager->endStationThai = ztp.getPara("EndStation_th");
            }
            comBroadcastManager->start();

//            QString str =  ztp.getPara("StartStation_th");
//            qDebug()<<str;
//            for(int i = 0; i< str.length();i++)
//            {
//                qDebug("%d : %x",i,str[i].unicode());
//            }
        }
    }
    else if(ztp.getPara("T") == "SLM_STATE")
    {
        ZTPprotocol ztpp = ztp;
        procSlmInfo(ztpp);
    }
    else if(ztp.getPara("T") == "TEST")
    {
        qDebug("recv test pkg");
        rmc.speed = ztp.getPara("speed").toDouble();
    }
    else if(ztp.getPara("T") == "GPS_GET")
    {
        ZTPprotocol ztpp;
        ztpp.addPara("T","GPS_ACK");
        ztpp.addPara("LNG",QString::number(rmc.longitude,'f',6));
        ztpp.addPara("LAT",QString::number(rmc.latitude,'f',6));
//        ztpp.addPara("ALTI",QString::number(rmc.speed));
        ztpp.addPara("SPEED",QString::number(rmc.speed));
        ztpm->SendOneZtp(ztpp,QHostAddress(ztp.getPara("RemoteHost")),ztp.getPara("RemotePort").toInt());
    }
//    ztp.print();
//    qDebug("leave file:%s line:%d func:%s thread:%ld",__FILE__,__LINE__,__FUNCTION__,QThread::currentThreadId());
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave onRecvNetMsg");
}
void WorkThread::onSendSysBroadcast()
{
    QTime t;
    double dist  = 0;
    ResultSet& _lineInfoSet = GlobalInfo::getInstance()->lineInfoSet;
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry onSendSysBroadcast");
//    qDebug("entry file:%s line:%d func:%s thread:%ld",__FILE__,__LINE__,__FUNCTION__,QThread::currentThreadId());
    ResultSet lineInfos = GlobalInfo::getInstance()->lineInfoSet;
    ZTPprotocol ztp;
    ztp.addPara("T","G_INFO");    //消息类型
    ztp.addPara("TrainNum",QString::number(GlobalInfo::getInstance()->train_id));    //发送者身份标识
    ztp.addPara("DateTime",rmc.dateTime.toString("yy-MM-dd hh:mm:ss"));
    if(rmc.speed < 2.5)
            ztp.addPara("Speed",QString::number(0));
    else
        ztp.addPara("Speed",QString::number(rmc.speed));

    int curIndex = GlobalInfo::getInstance()->curStationIndex;
    if(GlobalInfo::getInstance()->lineInfoSet.count() == 0)
    {
        QString startStationEN = "START";
        QString startStationThai = QString::fromUtf8("เริ่มต้นที่สถานี");
        QString endStationEN = "END";
        QString endStationThai = QString::fromUtf8("สุดท้ายยืน");

        ztp.addPara("CurrentStationEn","--");
        ztp.addPara("CurrentStationTh","--");
        ztp.addPara("StartStation",startStationEN);//始发站
        ztp.addPara("StartStation_th",startStationThai);
        ztp.addPara("EndStation",endStationEN);//终点站
        ztp.addPara("EndStation_th",endStationThai);
        ztp.addPara("NextStation","--");
        ztp.addPara("NextStation_th","--");
        ztp.addPara("NextStationTime","--");
        ztp.addPara("PreStation","--");
        ztp.addPara("PreStation_th","--");
        goto label;
    }

    if(curIndex < 0)
        return;
    //仅当当前位置在当前站点500米范围内时才显示当前站。
    dist = BhcTool::geogToDistance(rmc.longitude_nh,rmc.latitude_nh,_lineInfoSet[curIndex].getParaDouble("lng"),_lineInfoSet[curIndex].getParaDouble("lat"));
    if( dist < 0.5 )
    {
        t = QTime::fromString(_lineInfoSet[curIndex].getPara("start_time"),"hh:mm");
        if(t.isValid())
        {
            int secs_off = t.secsTo(QTime::currentTime());
            if(secs_off/60/60 < 16)//如果当前时间比起时刻表提前超过16小时,则认为是时间回滚,需要时刻表时间+1天
            {
                secs_off += 60*60*24;
            }
            if(secs_off/60 > 1) //迟出发1分钟以上
                GlobalInfo::getInstance()->secs_off = secs_off;
        }
        else
        {
            t = QTime::fromString(_lineInfoSet[curIndex].getPara("arrive_time"),"hh:mm");
            if(t.isValid())
            {

                int secs_off = t.secsTo(QTime::currentTime());
                if(secs_off/60/60 < 16)//如果当前时间比起时刻表提前超过16小时,则认为是时间回滚,需要时刻表时间+1天
                {
                    secs_off += 60*60*24;
                }
                if(secs_off/60 > 10) //呆在本站1分钟以上
                    GlobalInfo::getInstance()->secs_off = secs_off;
            }
        }
        ztp.addPara("SECS_OFF",QString::number(GlobalInfo::getInstance()->secs_off));
        ztp.addPara("CurrentStationEn",lineInfos[curIndex].getPara("station_name_en"));
        ztp.addPara("CurrentStationTh",lineInfos[curIndex].getPara("station_name_th"));
    }
    else
    {
//        curStationEmpty = true;
//        qDebug()<<"xxxxxxxxxxxxxxxxxxxx  "<<rmc.longitude_nh<<"  "<<rmc.latitude_nh<<"  "<<dist;
        ztp.addPara("CurrentStationEn","--");
        ztp.addPara("CurrentStationTh","--");
    }
    ztp.addPara("ArriveTime",lineInfos[GlobalInfo::getInstance()->lineInfoSet.count()-1].getPara("arrive_time"));//终点站到站时间
    t =QTime::fromString(lineInfos[GlobalInfo::getInstance()->lineInfoSet.count()-1].getPara("arrive_time"),"hh:mm");
    if(t.isValid())//如果满足时间条件 则覆盖ArriveTime
    {
        t = t.addSecs(GlobalInfo::getInstance()->secs_off);
        ztp.addPara("ArriveTime",t.toString("hh:mm"));
    }
    //
    ztp.addPara("StartStation",lineInfos[0].getPara("station_name_en"));//始发站
    ztp.addPara("StartStation_th",QString::fromUtf8(lineInfos[0].getPara("station_name_th").toUtf8()));
    ztp.addPara("EndStation",lineInfos[GlobalInfo::getInstance()->lineInfoSet.count()-1].getPara("station_name_en"));//终点站
    ztp.addPara("EndStation_th",QString::fromUtf8(lineInfos[GlobalInfo::getInstance()->lineInfoSet.count()-1].getPara("station_name_th").toUtf8()));


//    QString str =  ztp.getPara("StartStation_th");
//    str = str.toUtf8();
//    //QString str =  "นึ่รู้รู้รู้";
//    QByteArray bytes = ZTools::str2unicode(str);
//    //qDebug()<<str;
//    for(int i = 0; i< bytes.length();i++)
//    {
//        qDebug("%d : %x",i,bytes.data()[i]);
//    }


    if(curIndex == 0)
    {
        ztp.addPara("PreStation","--");
        ztp.addPara("PreStation_th","--");
        if(lineInfos.count() > 1)
        {
            ztp.addPara("NextStation",lineInfos[curIndex+1].getPara("station_name_en"));
            ztp.addPara("NextStation_th",lineInfos[curIndex+1].getPara("station_name_th"));
            ztp.addPara("NextStationTime",lineInfos[curIndex+1].getPara("arrive_time"));
            QTime t =QTime::fromString(lineInfos[curIndex+1].getPara("arrive_time"),"hh:mm");
            if(t.isValid())
            {
                t = t.addSecs(GlobalInfo::getInstance()->secs_off);
                ztp.addPara("NextStationTime",t.toString("hh:mm"));
            }

        }
        else
        {
            ztp.addPara("NextStation","--");
            ztp.addPara("NextStation_th","--");
            ztp.addPara("NextStationTime","--");
        }
    }
    else if(curIndex < GlobalInfo::getInstance()->lineInfoSet.count() - 1)
    {
        ztp.addPara("PreStation",lineInfos[curIndex-1].getPara("station_name_en"));
        ztp.addPara("PreStation_th",lineInfos[curIndex-1].getPara("station_name_th"));
        if(lineInfos.count() > 1)
        {
            ztp.addPara("NextStation",lineInfos[curIndex+1].getPara("station_name_en"));
            ztp.addPara("NextStation_th",lineInfos[curIndex+1].getPara("station_name_th"));
            ztp.addPara("NextStationTime",lineInfos[curIndex+1].getPara("arrive_time"));
            QTime t =QTime::fromString(lineInfos[curIndex+1].getPara("arrive_time"),"hh:mm");
            if(t.isValid())
            {
                t = t.addSecs(GlobalInfo::getInstance()->secs_off);
                ztp.addPara("NextStationTime",t.toString("hh:mm"));
            }
        }
        else
        {
            ztp.addPara("NextStation","--");
            ztp.addPara("NextStation_th","--");
            ztp.addPara("NextStationTime","--");
        }

    }
    else
    {
        ztp.addPara("PreStation",lineInfos[curIndex-1].getPara("station_name_en"));
        ztp.addPara("PreStation_th",lineInfos[curIndex-1].getPara("station_name_th"));
        ztp.addPara("NextStation","--");
        ztp.addPara("NextStation_th","--");
        ztp.addPara("NextStationTime","--");
    }
label:
    ztpm->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),8311);
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave onSendSysBroadcast");

}
//void WorkThread::gpsProccess()
//{
//    if(GlobalInfo_t::getInstance()->gps_send_flag == false)
//        return;
//    QString imgName = "/tmp/tmp_gpsmap.png";
//    mapManager->saveImage(1280,720,imgName);
//    QFile f(imgName);
//    if(!f.open(QFile::ReadOnly))
//    {
//        qDebug()<<"open the map file failed";
//    }

//    ZTPprotocol ztp;
//    ztp.addPara("T","GPS_map");
//    //ztp.addPara("name",imgName);
//    QByteArray buff = f.readAll();
//    f.close();
//    ztp.addPara("content",buff,ZTPprotocol::FILE);
//    ztpBroadcastManager->SendOneZtp(ztp,QHostAddress("224.102.228.40"),7878);

//}

void WorkThread::readGPS()
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry readGPS");
    gpsSoftDog->feed();
    if(gpsDevice == NULL)
        return;
    rmc = gpsDevice->getOneRPRMC();
    if(!rmc.isValid())
        return;
    rmc.print();
//    if(rmc.satelliteNr >= 5)
//    {
//       QString dateTime = rmc.dateTime.toString("yyyyMMdd HH:mm:ss");
//       QString str = QString("date -s \"%1\" && hwclock -w ").arg(dateTime);
//        system(str.toLatin1().data());
//    }

//    if(rmc.latitude < -20.499064 || rmc.latitude > -5.648786 ||
//            rmc.longitude < 97.283936 || rmc.longitude > 105.611572)
//        return;
    //QTime time;
    //time.start();
    //qDebug()<<"readGPS 1:"<<time.elapsed();
    if(GlobalInfo::getInstance()->lineInfoSet.count() <= 0 )
        return;
    static int indexNu = GlobalInfo::getInstance()->lineInfoSet[0].getParaInt("indexNu");
    if(rmc.latitude > -20.499064 && rmc.latitude < -5.648786 &&
            rmc.longitude > 97.283936 && rmc.longitude < 105.611572)
    {
        QTime time;
        time.start();
        rmc.longitude_nh=rmc.longitude;//这里拟合前后的经纬度做了区分以防竞态（实际上这里应该不会有竞态问题）。
        rmc.latitude_nh=rmc.latitude;
        BhcTool::fitGPSPoint(&rmc.longitude_nh,&rmc.latitude_nh,&GlobalInfo::getInstance()->curStationIndex,&indexNu,rmc.speed);
        qDebug("fitGPSPoint %d",time.elapsed());
        if(indexNu == -1)
            return;
    }
    else
    {
        indexNu = -1;
        return;
    }
    //qDebug()<<"readGPS 2:"<<time.elapsed();
//    for(int i = 0;i < GlobalInfo::getInstance()->lineInfoSet.count();i++)
//    {
//        const ResultRow& stationInfo = GlobalInfo::getInstance()->lineInfoSet[i];
//        if(sqrt(pow(rmc.longitude-stationInfo.getPara("lng").toDouble(),2)+pow(rmc.latitude-stationInfo.getPara("lat").toDouble(),2)) < 0.005)//约500米内   0.001度约合111.136米
//        {
//            GlobalInfo::getInstance()->curStationIndex = i;
//            break;
//        }
//    }
    ZTPprotocol ztp;
    ztp.addPara("T","GPS_map");
    ztp.addPara("lng",QString::number(rmc.longitude_nh,'f',6));
    ztp.addPara("lat",QString::number(rmc.latitude_nh,'f',6));

    ztp.addPara("satelliteNr",QString::number(rmc.satelliteNr));
    ztp.addPara("DateTime",rmc.dateTime.toString("yyyy-MM-dd hh:mm:ss"));
    ztp.addPara("Speed",QString::number(rmc.speed));
    ztp.addPara("cur_index",QString::number(indexNu));
    ztpm->SendOneZtp(ztp,QHostAddress("224.102.228.40"),7878);

//    ztp.print();
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave readGPS");
}
void WorkThread::test()
{
    qDebug("xxxxxxxxxxxx");
    ZTPManager* ztpm = dynamic_cast<ZTPManager*>(sender());
    qDebug()<<ztpm;
    if(ztpm != NULL)
    {
        ZTPprotocol ztp;
        ztpm->getOneZtp(ztp);
        ztp.print();
    }
}
static void modbusDeal()
{
    temp = 0;
    outTemp = 0;
    if(GlobalInfo::getInstance()->VServerIP != "")
    {
        ZTPManager* ztpm = new ZTPManager;
        int _tryCnt = 3;
        while(_tryCnt--)
        {
            ZTPprotocol ztp;
            ztp.addPara("T","Error");
            ztp.addPara("Code","103");
            ztpm->SendOneZtp(ztp,QHostAddress(GlobalInfo::getInstance()->VServerIP),8881);
            ZTPManager::ResultState res =  ztpm->waitOneZtp(ztp);
            if(res == ZTPManager::SUCCESS)
            {
                qDebug("Send Error 103 success!");
                break;
            }
        }
        delete ztpm;
    }
    //GlobalInfo::getInstance()->workThread->comBroadcastManager = NULL;
    qDebug("cleanUp with modbusDeal");
}

static void sendTemp()
{
    static ZTPManager* ztpm = new ZTPManager;
    ZTPprotocol ztp;
    ztp.addPara("T","CarTemperature");
    ztp.addPara("VALUE",    QString::number(temp));
    ztp.addPara("CAR_ID",QString::number(ZTools::getCarID()));
    ztpm->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),8312);
//    ztp.print();

    ztp.clear();
    ztp.addPara("T","OutTemperature");
    ztp.addPara("VALUE",    QString::number(outTemp));
    ztp.addPara("CAR_ID",QString::number(ZTools::getCarID()));
    //ztp.addPara("CAR_ID",QString::number(_ac->datList[0]));
    ztpm->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),8313);
    if(debug)
    {
        ztp.print();
    }

}
void WorkThread::recvModbus()
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry recvModbus");
    modbusDog->feed();
    Modbus* _ac = modbusManager->getOneModbus();
    if(debug)
    {
        for(int i =0; i < _ac->datList.length();i++)
        {
            qDebug()<<"datList "<<i<<" : "<<_ac->datList[i];
        }
    }
    if(_ac->datList.length() == 8)
    {
        temp = _ac->datList[4]*0.1-30;
        outTemp = _ac->datList[5]*0.1-30;
    }
    delete _ac;
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave recvModbus");
}
static void comDeal()
{
    if(GlobalInfo::getInstance()->VServerIP != "")
    {
        ZTPManager* ztpm = new ZTPManager;
        int _tryCnt = 3;
        while(_tryCnt--)
        {
            ZTPprotocol ztp;
            ztp.addPara("T","Error");
            ztp.addPara("Code","102");
            ztpm->SendOneZtp(ztp,QHostAddress(GlobalInfo::getInstance()->VServerIP),8881);
            ZTPManager::ResultState res =  ztpm->waitOneZtp(ztp);
            if(res == ZTPManager::SUCCESS)
            {
                qDebug("Send Error 102 success!");
                break;
            }
        }
        delete ztpm;
    }
    //GlobalInfo::getInstance()->workThread->comBroadcastManager = NULL;
    qDebug("cleanUp with comDeal");

}
static GPRMC *g_rmc = NULL;
static void gpsDeal()
{
    if(g_rmc != NULL)
        g_rmc->clear();
    if(GlobalInfo::getInstance()->VServerIP != "")
    {
        ZTPManager* ztpm = new ZTPManager;
        int _tryCnt = 3;
        while(_tryCnt--)
        {
            ZTPprotocol ztp;
            ztp.addPara("T","Error");
            ztp.addPara("Code","104");
            ztpm->SendOneZtp(ztp,QHostAddress(GlobalInfo::getInstance()->VServerIP),8881);
            ZTPManager::ResultState res =  ztpm->waitOneZtp(ztp);//Z X R 此处ztpm为侦听某ip端口.
            if(res == ZTPManager::SUCCESS)
            {
                qDebug("Send Error 104 success!");
                break;
            }
        }
        delete ztpm;
    }
    qDebug("cleanUp with gpsDeal");
}


void procSlmInfo(const ZTPprotocol& ztp)
{
    int carID = ztp.getPara("CAR_ID").toInt();
    if(carID < 1 || carID > 13)
    {
        qDebug()<<"CAR"<<QString::number(carID)<<" not exsitent";
        return;
    }
    if(CarInfo::v_carInfos[carID] == NULL)
    {
        qDebug()<<"CAR"<<QString::number(carID)<<" not exsitent";
        return;
    }
    else
    {
        CarInfo::v_carInfos[carID]->setSlm(ztp.getPara("SLM1"),1);
        CarInfo::v_carInfos[carID]->setSlm(ztp.getPara("SLM2"),2);
    }
}

/*
void WorkThread::test()
{
    QTimer *t = new QTimer;
    delete t;
    t = NULL;
//    *(int *)1024 = 321;
    t->setInterval(1000);
//    static int i = 0;
//    void *p = malloc(1024*1000);
//    memset(p,'a',1024*1000);
//    if(i++ == 1000)
//        printf("%p\n",p);
//    QTimer::singleShot(100,this,SLOT(test()));

}
*/
void WorkThread::sendMapTableSwitch()
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry sendMapTableSwitch");
    static int fMapTable = 0;//fMapTable = true map; fMapTable = false tables;
//    fMapTable = 1;
    qDebug("sendMapTableSwitch");
    if(fMapTable % 3 == 0)
    {
        ZTPprotocol ztp;
        ztp.addPara("T","MapTable");
        ztp.addPara("flag","table");
        ztpm->SendOneZtp(ztp,QHostAddress("224.102.228.40"),7676);
        //ztp.print();
    }
    else if(fMapTable % 3 == 1)
    {
        ZTPprotocol ztp;
        ztp.addPara("T","MapTable");
        ztp.addPara("map_type","detail");
        ztp.addPara("flag","map");
        ztpm->SendOneZtp(ztp,QHostAddress("224.102.228.40"),7676);
        //ztp.print();
    }
    else if(fMapTable % 3 == 2)
    {
        ZTPprotocol ztp;
        ztp.addPara("T","MapTable");
        ztp.addPara("flag","map");
        ztp.addPara("map_type","thumbnail");
        ztpm->SendOneZtp(ztp,QHostAddress("224.102.228.40"),7676);
        //ztp.print();
    }
    fMapTable++;
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave sendMapTableSwitch");
}
void WorkThread::timerEvent(QTimerEvent*event)
{
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry timerEvent");
    if(event->timerId() == sendTempTimerId)
    {
        sendTemp();
    }
    if(event->timerId() == mapTableSwitch_timer_id)
        sendMapTableSwitch();
    if(event->timerId()==hwCLockId)
    {
        if(rmc.satelliteNr >= 5)
        {
           QString dateTime = rmc.dateTime.toString("yyyy-MM-dd HH:mm:ss");
           QString str = QString("date -s \"%1\" && hwclock -w ").arg(dateTime);
           qDebug()<<"exec "<<str;
           system(str.toLatin1().data());
        }
    }
    if(event->timerId()==carrierClockId)
    {
        static unsigned int seq = 0;
        static ZTPManager* ztpm = new ZTPManager;
        ZTPprotocol ztp;
        ztp.addPara("T","CarrierHeart");
        ztp.addPara("MAGIC",QString::number(seq++));
        ztpm->SendOneZtp(ztp,QHostAddress(BROADCAST_IP),8317);
        ztpm->SendOneZtp(ztp,QHostAddress("192.168.5.10"),8888);
        qDebug("Send CarrierHeart.");
    }


    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave timerEvent");
}
//void WorkThread::test()
//{
//    static QString str = "OPEN";
//    static QString str2 = "OPEN";
//    if(str == "OPEN")
//    {
//        str = "CLOSE";
//        str2 = "OPEN";
//    }
//    else
//    {
//        str = "OPEN";
//        str2 = "CLOSE";
//    }
//    static int len = 0;
//    for(int i = 0;i < len;i++)
//    {
//        QTimer* t = new QTimer;
//        t->start(3000);
//        delete t;
//    }
//    len++;
//    CarInfo::v_carInfos[5]->setSlm(str,1);
//    CarInfo::v_carInfos[5]->setSlm(str2,2);
//    qDebug("xxxxxxxxxx");
//}

void WorkThread::recvDevInfo()
{

    ZTPprotocol ztp;
    ztpmRecvDevInfo->getOneZtp(ztp);
//    if(ztp.getPara("CARNO") == "9")
//        ztp.print();
    if(ztp.getPara("T") != "VERSION")
        return;
    QString ip = ztp.getPara("RemoteHost");
    quint32 key =QHostAddress(ip).toIPv4Address() ;
    if(!devMap.contains(key))
    {
        bool state = true;
        if(ztp.getPara("STATE") == "OFFLINE")
            state = false;
        else
            state = true;
        DevInfo* devInfo =  new DevInfo(ztp.getPara("DEVICE"),ip,ztp.getPara("MajorVersion").toInt(),ztp.getPara("MinorVersion").toInt(),ztp.getPara("MicroVersion").toInt(),state);
        connect(devInfo,SIGNAL(closeClient()),this,SLOT(deleteDev()),Qt::QueuedConnection);
        devMap.insert(key,devInfo);
    }
    else
    {
        devMap[key]->MajorVersion = ztp.getPara("MajorVersion").toInt();
        devMap[key]->MinorVersion = ztp.getPara("MinorVersion").toInt();
        devMap[key]->MicroVersion = ztp.getPara("MicroVersion").toInt();
        if(ztp.getPara("STATE") == "ONLINE")
        {
            devMap[key]->setOnline(true);
            devMap[key]->updateTimer();
        }
        else if(ztp.getPara("STATE") == "OFFLINE")
            devMap[key]->setOnline(false);
    }
}
void WorkThread::deleteDev()//操作devMap,由于在同一个线程增删不用做线程同步
{
    DevInfo*dev = (DevInfo*)sender();
    quint32 key =QHostAddress(dev->DevIp).toIPv4Address() ;
    delete devMap[key];
    devMap.remove(key);
}
void WorkThread::run()
{
    ztpmRecvDevInfo = new ZTPManager(8318,QHostAddress("224.102.228.40"));
    connect(ztpmRecvDevInfo,SIGNAL(readyRead()),this,SLOT(recvDevInfo()));

    sendTempTimerId = startTimer(2000);
//    QTimer* t = new QTimer;
//    t->setInterval(100);
//    t->start();
//    connect(t,SIGNAL(timeout()),this,SLOT(test()));
    qDebug("run");
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"ready entry run");
    //QTimer::singleShot(3000,this,SLOT(test()));
    ztpm = new ZTPManager(3320,QHostAddress("224.102.228.40"));//塞拉门状态
    connect(ztpm,SIGNAL(readyRead()),this,SLOT(onRecvNetMsg()));
    ztpm = new ZTPManager;//专用来发送包

    ZTPManager* g_infoZtpm = new ZTPManager(8311,QHostAddress(BROADCAST_IP));
    connect(g_infoZtpm,SIGNAL(readyRead()),this,SLOT(onRecvNetMsg()));//G_INFO包中仅更新广播控制器串口数据缓存，可以放在子线程。
    modbusManager = new ModbusManager("/dev/ttyUSB1",4);
    modbusDog = new SoftWatchdog(modbusManager,modbusDeal);
    connect(modbusManager,SIGNAL(readyRead()),this,SLOT(recvModbus()),Qt::DirectConnection);
    bool res = modbusManager->open();
    if(res == true)
    {
        qDebug("open modbusManager success.");
    }
    else
        qDebug("open modbusManager failed.");

    mapTableSwitch_timer_id = startTimer(10000);
    if(mapTableSwitch_timer_id == 0)
        qDebug("Create mapTableSwitch_timer_id falied!!");
    else
        qDebug("Create mapTableSwitch_timer_id success!!");
//    QTimer timerMapTableSwitch;
//    timerMapTableSwitch.setInterval(10000);
//    timerMapTableSwitch.setSingleShot(false);
//    connect(&timerMapTableSwitch,SIGNAL(timeout()),this,SLOT(sendMapTableSwitch()),Qt::DirectConnection);
//    timerMapTableSwitch.start();
    gpsDevice = new GPSDevice("/dev/ttymxc1");
    g_rmc = &rmc;
    gpsSoftDog = new SoftWatchdog(gpsDevice,gpsDeal);
//    SamplesList v;
//    v.push_back(GeogPixSample(1991,51,106.805015,-20.675571));
//    v.push_back(GeogPixSample(776,2730,100.120199,-6.423076));
//    v.push_back(GeogPixSample(861,1414,100.578835,-13.555069));
//    v.push_back(GeogPixSample(1743,951,105.426127,-15.991936));
//    mapManager = new GPSMapManager("/appbin/GPSMap.png",v);
//    mapManager->setLat(-11.555069);
//    mapManager->setLng(100.426127);
    connect(gpsDevice,SIGNAL(readyRead()),this,SLOT(readGPS()));
//    gpsTimer = new QTimer();
//    gpsTimer->setSingleShot(false);
//    gpsTimer->setInterval(3000);
    //connect(gpsTimer,SIGNAL(timeout()),this,SLOT(gpsProccess()));
//    gpsTimer->start();

    comBroadcastManager = new BroadcastManager("/dev/ttymxc2");
    comBroadcastManager->setPwdFlag(true);
    connect(comBroadcastManager,SIGNAL(readyRead()),this,SLOT(comBroadcastProc()));
    comSoftDog = new SoftWatchdog(comBroadcastManager,comDeal);

    ztpBroadcastManager = new ZTPManager(BROADCAST_PORT,QHostAddress(BROADCAST_IP));
    connect(ztpBroadcastManager,SIGNAL(readyRead()),this,SLOT(onRecvNetMsg()));

    //定时发送系统广播
    QTimer timerBroadcast;
    timerBroadcast.setSingleShot(false);
    timerBroadcast.setInterval(1000);
    timerBroadcast.start();
    connect(&timerBroadcast,SIGNAL(timeout()),this,SLOT(onSendSysBroadcast()));
    new ZDownloadThread;
    exec();
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave run");
}

void WorkThread::comBroadcastProc()
{
    static QString preBroadcast = "N";
    comSoftDog->feed();
    CallStateParser call = comBroadcastManager->getOneCallState();
    ZTPprotocol ztp;
    QString tmp;
    ztp.addPara("T","BroadCast");
    if(call.hasBroadcast)
        tmp = "Y";
    else
        tmp = "N";
    if(preBroadcast == "N" && tmp == "Y")
        GlobalInfo::getInstance()->vServerCtrl->pausedServer("PA");
    if(preBroadcast == "Y" && tmp == "N")
        GlobalInfo::getInstance()->vServerCtrl->playServer();
    preBroadcast = tmp;
}
