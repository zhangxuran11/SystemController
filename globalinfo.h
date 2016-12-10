#ifndef GLOBALINFO_H
#define GLOBALINFO_H
#include<QMap>
#include<QString>
#include<QTime>
#include"ztpmanager.h"
#include"dbservice.h"
#include"modbusmanager.h"
#include"zdebugcallstack.h"
#include"vserverctrl.h"
class MainWindow;
class WorkThread;
class QString;

class GlobalInfo
{
    static GlobalInfo* _instance;
    explicit GlobalInfo();
public:
    int secs_off;//列车运营时间偏移,单位秒.
    VServerCtrl* vServerCtrl;
    QTime time;
    ZDebugCallStack debugStack;
    WorkThread* workThread;
    static GlobalInfo* getInstance();

    QString VServerIP;
    ResultSet lineInfoSet;
    int train_id;
    int curStationIndex;
    MainWindow* mainWin;
//    static void loadCarNuMap();
//    static void insertCarNuMap();
};

#endif // GLOBALINFO_H
