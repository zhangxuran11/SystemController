#ifndef BHCTOOL_H
#define BHCTOOL_H

#include <QObject>
#include"ztool.h"
class BhcTool : public QObject
{
    Q_OBJECT
public:
    explicit BhcTool(QObject *parent = 0);
    static int getGlobalCarNu(); //获得车厢号 －1为错误
    static QByteArray readLineFromFile(const QString& fileName,int lineNo){return ZTool::readLineFromFile(fileName,lineNo);}
    static void fitGPSPoint(qreal *lng,qreal *lat,int *index,int *indexNu,double speed);
    static double geogToDistance(double lngA,double latA,double lngB,double latB);//format : ddd.ddddd(lng), dd.ddddd(lat)   有符号，西负东正，北负南正。
    
signals:
    
public slots:
    
};

#endif // BHCTOOL_H
