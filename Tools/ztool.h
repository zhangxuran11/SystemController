#ifndef ZTOOL_H
#define ZTOOL_H
#include<QtGlobal>
class ZTool
{
    ZTool();
public:
    static char* hex2ascii(int hex,char *,int len);
    static quint64 ascii2hex(const char *ascii,int len);
    static void msleep(int ms);
    static QByteArray readLineFromFile(const QString& fileName,int lineNo);
    static int getCarID();
};

#endif // ZTOOL_H
