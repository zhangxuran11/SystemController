#include "bhctool.h"
#include <QFile>
#include<QRegExp>
#include<globalinfo_t.h>
#include<globalinfo.h>
#include<math.h>
BhcTool::BhcTool(QObject *parent) :
    QObject(parent)
{
}
//返回距离单位Km
double BhcTool::geogToDistance(double lngA,double latA,double lngB,double latB)//format : ddd.ddddd(lng), dd.ddddd(lat)   有符号，西负东正，北负南正。
{
    double MLatA = latA;
    double MLonA = lngA;
    double MLatB = latB;
    double MLonB = lngB;
    MLatA *= (M_PI/180);
    MLonA *= (M_PI/180);
    MLatB *= (M_PI/180);
    MLonB *= (M_PI/180);
    double R = 6371.004;
//            d=R*arcos[cos(Y1)*cos(Y2)*cos(X1-X2)+sin(Y1)*sin(Y2)];
    double d=R*acos(cos(MLatA)*cos(MLatB)*cos(MLonA-MLonB)+sin(MLatA)*sin(MLatB));
    return d;
}

int BhcTool::getGlobalCarNu()
{
    QByteArray line = readLineFromFile("/etc/network/interface",25);
    if(line == "")
        return -1;
    QRegExp reg(QString("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})"));
    int pos = reg.indexIn ( line);
    if(pos == -1)
        return -1;
    int len = reg.matchedLength();
    line = line.mid(pos,len);
    int g_carNu = line.split('.')[2].toInt();
    return g_carNu;
}

static int getCurStationInd(qreal lng,qreal lat);
//精度,纬度,当前站内存索引,当前站数据库索引,速度
void BhcTool::fitGPSPoint(qreal *lng,qreal *lat,int *index,int *indexNu,double )
{
    static int top = 0,len = -1;
    static double lng_nh = 0;//最后一次拟合
    static double lat_nh = 0;
    static int train_id = GlobalInfo_t::curTrainId;
    static int nopCnt = 0;
    double rowLng = *lng;//存储原始经纬度。
    double rowLat = *lat;

    if( nopCnt > 0)
    {
        *lng = lng_nh;//如果不拟合先让lng,lat等于最后一次拟合结果
        *lat = lat_nh;
        *indexNu = -1;
        nopCnt--;
        return;
    }
    if(train_id != GlobalInfo_t::curTrainId)
    {
    train_id = GlobalInfo_t::curTrainId;
    top = 0;
    len = -1;
    }
    QString table = QString("(select lat,lng,rowid,station_id from tb_railway_info where train_id=%1 and rowid>%2 limit %3)").arg(train_id).arg(top).arg(len);
    QString ni_he_railway_table = QString("select (%1-lat)*(%1-lat)+(%2-lng)*(%2-lng) ind,lat,lng,rowid,station_id from %3  order by ind limit 1").arg(QString::number(*lat,'f',6)).arg(QString::number(*lng,'f',6)).arg(table);
    //    qDebug()<<ni_he_railway_table;
    ResultSet ni_he_railway_set = GlobalInfo_t::getInstance()->db->query(ni_he_railway_table);
    lat_nh = ni_he_railway_set[0].getParaDouble("lat");
    lng_nh = ni_he_railway_set[0].getParaDouble("lng");
    *lat = lat_nh;
    *lng = lng_nh;


    top = ni_he_railway_set[0].getParaInt("rowid") - 500;
    top = top < 0? 0:top;
    len = 1000;
    if(geogToDistance(rowLng,rowLat,ni_he_railway_set[0].getParaDouble("lng"),ni_he_railway_set[0].getParaDouble("lat")) > 2)//如果距离过大(大于2公里)则下次在所有采样点中拟合。
    {
        top = 0;
        len = -1;
        nopCnt = 20;
    }

    *index = getCurStationInd(*lng,*lat);
    *indexNu = GlobalInfo::getInstance()->lineInfoSet[*index].getParaInt("indexNu");
}

static int getCurStationInd(qreal lng,qreal lat)
{
    ResultSet& _lineInfoSet = GlobalInfo::getInstance()->lineInfoSet;
    static int train_id = GlobalInfo_t::curTrainId;
    static int curStation = -1;

    if(train_id != GlobalInfo_t::curTrainId)
    {
        train_id = GlobalInfo_t::curTrainId;
        curStation = -1;
    }
    //    enum ESTATE{INIT,GET_CUR_STATION};
    if(curStation == -1)
    {
        QString station_table = QString("select (%1-lat)*(%1-lat)+(%2-lng)*(%2-lng) ind,indexNu,lng,lat  from tb_station_info where  train_id=%3 order by ind limit 1;").arg(lat).arg(lng).arg(train_id);
        ResultSet station_set = GlobalInfo_t::getInstance()->db->query(station_table);
        int ind = station_set[0].getParaInt("indexNu");
        for(int i = 0;i < _lineInfoSet.count();i++)
        {
            if( _lineInfoSet[i].getParaInt("indexNu") == ind)//此处不可直接以ind为索引lineInfoSet，中途可能临时增删站点导致indexNu不连续。
            {
                curStation = i;
                break;
            }
        }

        if(curStation == 0) //当前为第一站
            return curStation;
        //A_B离当前位置最近站与上一站距离
        double A_B = BhcTool::geogToDistance(_lineInfoSet[curStation].getParaDouble("lng"),_lineInfoSet[curStation].getParaDouble("lat"),_lineInfoSet[curStation-1].getParaDouble("lng"),_lineInfoSet[curStation-1].getParaDouble("lat"));
        //C_B   当前点到拟合站上一站距离
        double C_B = BhcTool::geogToDistance(lng,lat,_lineInfoSet[curStation-1].getParaDouble("lng"),_lineInfoSet[curStation-1].getParaDouble("lat"));
        if(A_B > C_B)
            curStation--;
        return curStation;
    }
    else//curStation != -1
    {
        for(int i = curStation+1;i < _lineInfoSet.count();i++)
        {
            if(BhcTool::geogToDistance(lng,lat,_lineInfoSet[i].getParaDouble("lng"),_lineInfoSet[i].getParaDouble("lat")) <    0.5 )
            {
                curStation = i;
                break;
            }
        }
        return curStation;
    }
}
