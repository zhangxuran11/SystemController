#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>
#include<QTimerEvent>
#include "globalinfo.h"
#include"gpsdevice.h"
#include"broadcastmanager.h"
#include"softwatchdog.h"
#include "modbusmanager.h"
#include "iodevicelistener.h"
#include"devinfo.h"
#include<QMap>
class GPSDevice;
class QTimer;
class ZTPManager;
class WorkThread : public QThread
{
    Q_OBJECT
    QMap<int,DevInfo*> devMap;
    ModbusManager*  modbusManager;
    SoftWatchdog* modbusDog;
    QTimer* gpsTimer;
    GPSDevice* gpsDevice;

    ZTPManager* ztpBroadcastManager;
//    GPSMapManager* mapManager;
    GPRMC rmc;
    ZTPManager* ztpm;
    ZTPManager* slmZtpm;
    ZTPManager* ztpmRecvDevInfo;

public:

    BroadcastManager* comBroadcastManager;
    SoftWatchdog* gpsSoftDog;
    SoftWatchdog* comSoftDog;
    explicit WorkThread(QObject *parent = 0);
    ~WorkThread();
private:
    int mapTableSwitch_timer_id;
    int hwCLockId;
    int carrierClockId;
    
signals:
protected:
    void timerEvent(QTimerEvent* event);
private slots:
    void test();
    void comBroadcastProc();
    void run();
    void onSendSysBroadcast();
    void onRecvNetMsg();
//    void gpsProccess();
    void readGPS();
    void recvModbus();
    void sendMapTableSwitch();
    void recvDevInfo();
    void deleteDev();
};

#endif // WORKTHREAD_H
