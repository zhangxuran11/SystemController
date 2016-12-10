#ifndef DEVLIST_H
#define DEVLIST_H
#include<QList>
class DevList
{
    QList<Device_Info> devList;
public:
    DevList();
    int length()const{return devList.length();}
    const Device_Info& at(int i)const{return devList[i];}
    const Device_Info& operator[](int i)const{return devList[i];}
};

#endif // DEVLIST_H
