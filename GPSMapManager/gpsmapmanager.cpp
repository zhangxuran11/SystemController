#include "gpsmapmanager.h"
#include <cmath>
#include<QPainter>
#include <algorithm>




GPSMapManager::GPSMapManager(const QString& _mapPath,const SamplesList &_samples)
    :samples(_samples),mapPath(_mapPath)
{
    mapImage.load(_mapPath);
}
QPointF GPSMapManager::geog2pix(double lng,double lat)
{
    const SamplesList refPoints = findSample(lng,lat);
	double x_lng_factor = (refPoints[1].pix_x-refPoints[0].pix_x)/(refPoints[1].lng-refPoints[0].lng);
	double y_lat_factor = (refPoints[1].pix_y-refPoints[0].pix_y)/(refPoints[1].lat-refPoints[0].lat);

    double x = refPoints[0].pix_x + (lng - refPoints[0].lng) * x_lng_factor;
    double y = refPoints[0].pix_y + (lat - refPoints[0].lat) * y_lat_factor;
    return QPointF(x,y);
}
const SamplesList GPSMapManager::findSample(double lng,double lat)
{
    SamplesList v;
	GeogPixSample* sample1 = &samples[0];
	GeogPixSample* sample2 = &samples[1];
	double dist1 = 180,dist2 = 180;
    for(SamplesList::iterator it = samples.begin(); it != samples.end(); it++)
	{
		double dist = sqrt(pow(it->lat - lat,2)+pow(it->lng - lng,2));
		if (dist < dist1)
		{
			dist2 = dist1;
			dist1 = dist;
			sample2 = sample1;
			sample1 = &(*it);
		}
		else if(dist < dist2)
		{
			dist2 = dist;
			sample2 = &(*it);
		}
		
	}
	v.push_back(*sample1);
	v.push_back(*sample2);
	return v;
}
const QImage& GPSMapManager::getImage()
{
    //mapImage.load(mapPath);

//    QPainter painter;
//    painter.begin(&mapImage);
//    QPen pen(painter.pen());
//    pen.setWidth(10);
//    pen.setColor(QColor(Qt::red));
//    painter.setPen(pen);
//    painter.drawPoint(geog2pix(lng,lat));
//    painter.end();
    return mapImage;
}
void GPSMapManager::saveImage(int width,int height,const QString& toFileName)
{
    QImage img(mapImage.copy(geog2pix(lng,lat).x()-width/2,geog2pix(lng,lat).y()-height/2, width,height));
    QPainter painter;
    painter.begin(&img);
    QPen pen(painter.pen());
    pen.setWidth(10);
    pen.setColor(QColor(Qt::red));
    painter.setPen(pen);
    painter.drawPoint(width/2,height/2);
    painter.end();

    img.save(toFileName);
}
