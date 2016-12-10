#ifndef CARINFO_H
#define CARINFO_H
#include<QString>
#include<QLabel>
#include<QVector>
#include<QTimer>
class CarInfo:public QObject
{
	Q_OBJECT
	QTimer timer;
private slots:
	void slot_timeout();
public:
    CarInfo();
    QLabel* slm1;
    QLabel* slm2;
    void setSlm(const QString& state,int slmNo);
    static QVector<CarInfo*> v_carInfos;
    static void init();
};

#endif // CARINFO_H
