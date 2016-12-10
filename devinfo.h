#ifndef DEVINFO_H
#define DEVINFO_H
#include <QString>
#include<QObject>
#include<QTimer>
class DevInfo:public QObject
{
    Q_OBJECT
public:
    QTimer timer;
    const QString DevName;
    const QString DevIp;
    int MajorVersion;
    int MinorVersion;
    int MicroVersion;
    int interval;
    bool f_online;
    DevInfo(QString devName,QString devIp,int majorVersion,int minorVersion,int microVersion,bool online);
    bool isOnline(){return f_online;}
    void setOnline(bool state){f_online = state;}

    void updateTimer(){timer.start(interval);}//当收到客户端传来版本包STATE为ONLINE时调用
signals:
    void closeClient();
private slots:
    void slot_timeout();
};

#endif // DEVINFO_H
