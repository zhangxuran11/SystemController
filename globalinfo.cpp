#include "globalinfo.h"
#include<QSettings>
#include<QStringList>
#include<QMap>
#include<globalinfo_t.h>
#include"config.h"
#include<QFile>


GlobalInfo* GlobalInfo::_instance = NULL;
GlobalInfo::GlobalInfo()
{

    debugStack.setListLen(1000);
    curStationIndex = 0;
    VServerIP = "";
    secs_off = 0 ;
    time.start();
    vServerCtrl = new VServerCtrl;
}
GlobalInfo* GlobalInfo::getInstance()
{
    if(_instance == NULL)
        _instance = new GlobalInfo();
	return _instance;
}
