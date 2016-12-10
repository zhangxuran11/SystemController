#ifndef ZDOWNLOADTHREAD_H
#define ZDOWNLOADTHREAD_H
#include <QThread>
#include<QMap>
#include<QList>
#include"ztpmanager.h"
class ZTask:public QObject
{
    Q_OBJECT
public:
    enum E_STATE{DISABLE,IDLE,BUSY};
    QString c_ip;//客户机IP
    int c_port;
    QString resourcePath;//被请求资源基于/srv/tftp/路经.
    E_STATE state;

    explicit ZTask(const QString& ip,int port,const QString& ressource)
    {c_ip = ip;c_port = port,resourcePath = ressource; state = BUSY;}
public slots:
    void onComplete(){state = IDLE;}
};
class ZDownloadThread : public QThread
{
    Q_OBJECT
    QMap<QString,ZTask*> clientMap;
    QList<const ZTask *> taskList;
    ZTPManager* ztpm;
public:
    ZDownloadThread();
protected slots:
    void run();
private slots:
    void OnProcReq();
};

#endif // ZDOWNLOADTHREAD_H
