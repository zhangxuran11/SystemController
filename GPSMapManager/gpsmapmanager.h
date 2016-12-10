#ifndef GPSMAPMANAGER_H
#define GPSMAPMANAGER_H
#include<QPointF>
#include<vector>
#include<QImage>
#include "geogpixsample.h"
class GPSMapManager;


typedef std::vector<GeogPixSample> SamplesList;
class GPSMapManager
{
    SamplesList samples;
    const QString mapPath;
    QImage mapImage;

    double lng;
    double lat;
public:
    QPointF geog2pix(double lng,double lat);
    const QImage& getImage();
    GPSMapManager(){}
    GPSMapManager(const QString& mapPath,const SamplesList &_samples);
    double getLng()const {return lng;}
    double getLat()const{return lat;}
    void setLng(double _lng){lng = _lng;}
    void setLat(double _lat){lat = _lat;}
    void saveImage(int width,int height,const QString& toFileName);
private:
    const SamplesList findSample(double lng,double lat);
};

#endif // GPSMAPMANAGER_H
