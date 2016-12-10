#include "gprmc.h"
#include<QStringList>
#include<cmath>
#include <QDebug>
GPRMC::GPRMC(const QString& bytes)
{
    load(bytes);

}
GPRMC::GPRMC()
{
    clear();
}
void GPRMC::clear()
{
    _isValid = false;
    state = ' ';
    speed = 0;
    latitude = 0;
    longitude = 0;
    latitude_nh = 0;
    longitude_nh = 0;
    satelliteNr = 0;
    dateTime.setTime(QTime::currentTime());
    dateTime.setDate(QDate::currentDate());
}
void GPRMC::load(const QString& bytes)
{
    QStringList strList = bytes.trimmed().split(QChar(','));
    if(strList.length()<13)
        return;
    QStringList tmpList = strList[12].split(QChar('*'));
    if(tmpList.length()<2)
        return;
    quint8 refXor = (quint8)tmpList[1].toUInt(0,16);
    if(refXor != checkXor(bytes.toUtf8()))
    {
        qDebug("GPRMC Xor error \n");
        qDebug()<<bytes;
        qDebug("sim:%x : %x",refXor, checkXor(bytes.toUtf8()));
        return;
    }
    dateTime.setTime(QTime::fromString(strList[1]+"0","hhmmss.zzz"));
//    qDebug()<<"time : "<<strList[1];
    dateTime.setDate(QDate::fromString(strList[9].mid(0,4)+"20"+strList[9].mid(4,2),"ddMMyyyy"));
    dateTime = dateTime.addSecs(7*60*60);
//    qDebug("refXor %x,checkXor %x\n",refXor,checkXor(bytes.toUtf8()));
    _isValid = true;

    state = strList[2].toUtf8().data()[0];
    latitude = strList[3].toDouble();    //北负南正
    if(strList[4] == "N")
        latitude *= -1;

    longitude = strList[5].toDouble();   //西负东正
    if(strList[6] == "W")
        longitude *= -1;

    latitude = (int)latitude/100 + fmod(latitude,100)/60;
    longitude = (int)longitude/100 + fmod(longitude,100)/60;

//    latitude += 22.5060938;
//    longitude += -15.1757683;
    speed = strList[7].toDouble()*1.852;
}
void GPRMC::loadGGA(const QString& bytes)
{
    QStringList strList = bytes.trimmed().split(QChar(','));
    if(strList.length()<15)
        return;
    QStringList tmpList = strList[14].split(QChar('*'));
    if(tmpList.length()<2)
        return;
    quint8 refXor = (quint8)tmpList[1].toUInt(0,16);
    if(refXor != checkXor(bytes.toUtf8()))
    {
        qDebug("GPGGAXor error \n");
        qDebug()<<bytes;
        qDebug("sim:%x : %x",refXor, checkXor(bytes.toUtf8()));
        return;
    }
    satelliteNr = strList[7].toInt();
//    qDebug("refXor %x,checkXor %x\n",refXor,checkXor(bytes.toUtf8()));
    _isValid = true;
}
quint8 GPRMC::checkXor(QByteArray origin)
{
    quint8 xorSum = 0;
    char *off = origin.data();
    while(*(++off)!= '*')
        xorSum ^= *off;
    return xorSum;
}

void  GPRMC::print()const
{
    qDebug("GPS Info :");
    qDebug()<<"isValid  :"<<isValid();
    qDebug()<<"state    :"<<state;
    qDebug()<<"satelliteNr :"<<satelliteNr;
    qDebug()<<"latitude :"<<QString::number(latitude,'f',6);
    qDebug()<<"longitude :"<<QString::number(longitude,'f',6);

    qDebug("speed :%.7f",speed);
    qDebug()<<"date time : "<<dateTime.toString();
//    qDebug("latitude :%.5f longitude :%.5f",fmod(latitude,100),fmod(longitude,100));
//    qDebug("latitude :%d longitude :%d",abs((int)latitude/100*100),abs((int)longitude/100*100));
//    qDebug("latitude :%.7f longitude :%.7f",(int)latitude/100 + fmod(latitude,100)/60,(int)longitude/100 + fmod(longitude,100)/60);
}


