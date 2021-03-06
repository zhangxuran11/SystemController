#ifndef VERSIONSENDER_H
#define VERSIONSENDER_H
#include <QTimer>
class VersionSender : public QObject
{
    Q_OBJECT
    QTimer timer;
    const QString DevName;
    const int MajorVersion;
    const int MinorVersion;
    const int MicroVersion;
    int carNo;
    bool online;
public:
    void setOnline(bool f){online = f;}
    VersionSender(QString devName,int majorVersion,int minorVersion,int microVersion);
private slots  :
    void OnSendVersion();
};

#endif // VERSIONSENDER_H
