#include "zdownloadthread.h"
#include <QFile>

ZDownloadThread::ZDownloadThread()
{
    ztpm = new ZTPManager(8316,QHostAddress("224.102.228.40"));
    connect(ztpm,SIGNAL(readyRead()),this,SLOT(OnProcReq()));
    start();
}
void ZDownloadThread::OnProcReq()
{
    ZTPprotocol ztp;
    ZTask* task = NULL;
    ZTPManager* ztpm = dynamic_cast<ZTPManager*>(sender());

    ztpm->getOneZtp(ztp);
    qDebug("xxxxxxxxxxxxxxxxx");
    ztp.print();
    qDebug("xxxxxxxxxxxxx");
    if(ztp.getPara("T") == "Down")
    {
        QString resource = ztp.getPara("Resource");
        int port = ztp.getPara(REMOTE_PORT).toInt();
        if(port == 0)
            return;
        if(!QFile::exists("/srv/tftp/"+resource))
        {
            qDebug()<<QString("res %1 is not exists!").arg(resource);
            return;
        }
        QString clientIp = ztp.getPara(REMOTE_HOST);
        QString key = clientIp+":"+QString::number(port);
        if(!clientMap.contains(key))
        {
            task = new ZTask(clientIp,port,resource);
            taskList.append(task);
            clientMap.insert(key,task);
            return;
        }
        task = clientMap[key];
        task->resourcePath = resource;
        task->c_port = port;
        if(task->state == ZTask::IDLE && !taskList.contains(task))
        {
            task->state = ZTask::BUSY;
            taskList.append(task);
        }
    }
}
void ZDownloadThread::run()
{
    while(true)
    {
        if(taskList.count()>0)
        {
            const ZTask* task = taskList.takeFirst();
            char cmdbuf[100];
            memset(cmdbuf,0,100);
            QString cmdStr = QString("tftp  -p  -r %1  -l /srv/tftp/%1 %2").arg(task->resourcePath).arg(task->c_ip);
            sprintf(cmdbuf,"%s",cmdStr.toLatin1().data());
            qDebug()<<cmdbuf;
            system(cmdbuf);
            qDebug("xxxxxxxxxxx");

            ZTPprotocol ztp;
            ztp.addPara("T","DOWN_COMPLETE");
            ztpm->SendOneZtp(ztp,QHostAddress(task->c_ip),task->c_port);
            ztp.print();
            ZTask* v_task = const_cast<ZTask*>(task);
            QTimer::singleShot(5000,v_task,SLOT(onComplete()));
        }
        msleep(100);
    }
}
