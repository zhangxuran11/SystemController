#include "devinfo.h"
#include<QFile>
DevInfo::DevInfo(QString devName,QString devIp,int majorVersion,int minorVersion,int microVersion,bool online):
    DevName(devName),DevIp(devIp),MajorVersion(majorVersion),MinorVersion(minorVersion),MicroVersion(microVersion),f_online(online)
{
    interval = 10000;
    timer.setSingleShot(false);
    connect(&timer,SIGNAL(timeout()),this,SLOT(slot_timeout()));
    updateTimer();
}
void DevInfo::slot_timeout()
{
//    f_online = false;
    if(!f_online)//说明收到客户端版本包,但是版本包传来的是离线信息,否则认为是网络故障引起,不做处理
    {
        if(!QFile::exists("/tmp/kill.sh"))
        {
            QFile::copy(":/kill.sh","/tmp/kill.sh");
            system("chmod +x /tmp/kill.sh");
        }
        QString dev;
        if(DevName == "CarController")
            dev = "car";
        else if(DevName == "VideoSubServer")
            dev = "sub";
        else if(DevName == "RoomMedia")
            dev = "room";
        else
            return;
        QString cmd = "/tmp/kill.sh "+DevIp+" "+dev;
        system(cmd.toAscii().data());
        cmd = "echo "+cmd+" >> /kill_log.txt";
        //system(cmd.toAscii().data());
        emit closeClient();
    }
}
