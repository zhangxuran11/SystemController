#ifndef GPSDEVICE_H
#define GPSDEVICE_H
#include<QString>

#include"gprmc.h"
class QextSerialPort;


class GPSDevice :public QObject
{
    Q_OBJECT
    QextSerialPort *GPS_serial;
signals:
    void readyRead();
public:
    GPSDevice(const QString& portName,QObject*parent = 0);
    ~GPSDevice();
    GPRMC gprmc;
    const GPRMC getOneRPRMC();
private slots:
    void onReadyRead();

};
#endif // GPSDEVICE_H
