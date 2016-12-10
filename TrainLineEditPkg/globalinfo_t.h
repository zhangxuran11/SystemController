#ifndef GLOBALINFO_T_H
#define GLOBALINFO_T_H


#include"dbservice.h"
class LineWin;
class DetailInfoWin;
class GlobalInfo_t
{
    static GlobalInfo_t* _instance;

    GlobalInfo_t();
public:

    DBService *db;

    static GlobalInfo_t* getInstance();
    static void saveCurrentTrainIdToLocalFile(int trainId);
    static int getCurrentTrainIdFromLocalFile();
    static int curTrainId;

    LineWin* lineW;
    DetailInfoWin* detailW;

    //StationMap stationMap;
};

#endif // GLOBALINFO_T_H
