#include "ztool.h"
#include<QEventLoop>
#include<QTimer>
#include<ctype.h>
#include<QFile>
#include<QStringList>
#include<QNetworkInterface>
ZTool::ZTool()
{
}
QByteArray ZTool::readLineFromFile(const QString& fileName,int lineNo)
{
    QFile f(fileName);
    QByteArray line;
    f.open(QFile::ReadOnly);
    while(!f.atEnd()&&lineNo--)
        line = f.readLine();
    if (f.atEnd() && lineNo > 0)
        line = "";
    else
        line = line;
    f.close();
    return line;
}
char* ZTool::hex2ascii(int hex,char *ascii,int len)
{
    char* _ascii = ascii;
    while(len--)
        *ascii++ = ((hex >> (len << 2))&0x0f) > 9?((hex >> (len << 2))&0x0f) - 10 +'A':((hex >> (len << 2))&0x0f) + '0';
    return _ascii;

}

//    int h;
//    while(len--)
//        ascii++ = (h = ((hex >> (len << 2))&0x0f)) > 9?h - 10 +'a':h + '0';
quint64 ZTool::ascii2hex(const char *ascii,int len)
{
    quint64 hex = 0;
    while(len--)
        hex |=  isdigit(*ascii)?quint64(*ascii++ -'0')<<(len<<2):isalpha(*ascii)?quint64(tolower(*ascii++)-'a'+10)<<(len<<2):0<<(len<<2);
    return hex;
}
void ZTool::msleep(int ms)
{
    QEventLoop q;
    QTimer::singleShot(ms,&q,SLOT(quit()));
    q.exec();
}



int ZTool::getCarID()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list)
    {
        QString ip = address.toString();
        if(ip.startsWith("192.168."))
        {
            return ip.split(QChar('.'))[2].toInt();
        }
    }
    return 0;
}
