#include "gpsdevice.h"
#include<QStringList>
#include <QDebug>
#include<QThread>
#include"globalinfo.h"
#include "qextserialport/src/qextserialport.h"



static void QextSerialPort_init(QextSerialPort *port)
{
    port->setDtr();
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(500);

}
GPSDevice::GPSDevice(const QString& portName, QObject* parent):
    QObject(parent)
{
    GPS_serial= new QextSerialPort(portName,QextSerialPort::EventDriven);
    QextSerialPort_init(GPS_serial);
    if (GPS_serial->open(QIODevice::ReadWrite) == true) {
        qDebug()<<"listening for data on "<<GPS_serial->portName();
    }
    else {
        qDebug()<<"device failed to open: "<<GPS_serial->errorString();
    }
    connect(GPS_serial,SIGNAL(readyRead()),this,SLOT(onReadyRead()),Qt::DirectConnection);
}

void GPSDevice::onReadyRead()
{
    static ZTPManager* ztpm = new ZTPManager;
    static QString str;
    static QRegExp rx("\\*[0-9A-F]{2}");
    static int start = -1, pos = -1;
    QString buf = GPS_serial->readAll();
    str.append(buf);
    ZTPprotocol ztp;
    ztp.addPara("T","GPS_DEBUG");
    ztp.addPara("GPS_FRAME",buf);
    ztpm->SendOneZtp(ztp,QHostAddress("224.102.228.40"),8322);

//    qDebug()<<str;
//    qDebug("++++++++++++++++ 11111111111");
    if(start == -1)
        start = str.indexOf("$GP");
    if(start == -1)
    {
        //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave GPSDevice::onReadyRead");
        return;
    }
    if(pos == -1)
        pos = rx.indexIn(str,start);
    if(rx.matchedLength() < 3)
    {
        pos = -1;
        //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave GPSDevice::onReadyRead");
        return;
    }
    QString frame = str.mid(start,pos + rx.matchedLength() - start);
    str.remove(0,pos + rx.matchedLength());
    start = -1;
    pos = -1;
//    qDebug("================ 22222222");
//    qDebug()<<frame;
//    qDebug("++++++++++++++++ 2222222");
    if(frame.indexOf("$GPRMC") == 0)
    {
        gprmc.clear();
//        qDebug()<<frame<<"  ++++ "<<GlobalInfo::getInstance()->time.elapsed();
        gprmc.load(frame);
    }
    if(frame.indexOf("$GPGGA") == 0)
    {
        if(gprmc.isValid())
        {
//            qDebug()<<frame<<"  ++++ "<<GlobalInfo::getInstance()->time.elapsed();
            gprmc.loadGGA(frame);
        }
        if(gprmc.isValid())
        {
            emit readyRead();
        }
    }
    //GlobalInfo::getInstance()->debugStack.appendStack(QThread::currentThreadId(),"leave GPSDevice::onReadyRead");
}
const GPRMC GPSDevice::getOneRPRMC()
{
    GPRMC tmp = gprmc;
    gprmc.clear();
    return tmp;
}
GPSDevice::~GPSDevice()
{
    delete GPS_serial;
}
