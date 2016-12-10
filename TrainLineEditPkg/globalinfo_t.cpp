#include "globalinfo_t.h"
#include<QFile>
int GlobalInfo_t::curTrainId = -1;
GlobalInfo_t*  GlobalInfo_t::_instance = NULL;
GlobalInfo_t::GlobalInfo_t()
{
    db = new DBService("/srv/tftp/TrainLine.db");

}
GlobalInfo_t* GlobalInfo_t::getInstance()
{
    if(_instance == NULL)
        _instance = new GlobalInfo_t;
    return _instance;
}
int GlobalInfo_t::getCurrentTrainIdFromLocalFile()
{
    QFile f("/appbin/curTrainId.txt");
    f.open(QFile::ReadOnly);
    QString str = f.readAll();
    f.close();
    curTrainId = str.toInt();
    return str.toInt();
}
void GlobalInfo_t::saveCurrentTrainIdToLocalFile(int trainId)
{
    QFile f("/appbin/curTrainId.txt");
    curTrainId = trainId;
    f.open(QFile::Truncate|QFile::WriteOnly);
    f.write(QString::number(trainId).toAscii());
    f.close();
    system("sync");
}
