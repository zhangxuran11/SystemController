#define __MAIN__
#include <QtGui/QApplication>
#include "mainwindow.h"
//#include "global.h"
#include"form.h"
//#include"crashtool.h"
#include<QFile>
#include<QDebug>
int main(int argc, char *argv[])
{
    //EnableCrashHandler();
    QApplication a(argc, argv);
    QFile file(":qss/mainframe.qss");
    //QApplication::setOverrideCursor(Qt::BlankCursor);
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    file.close();

    qDebug("2016/3/24 18:48");
    MainWindow w;
    w.show();
//    Form f;
//    f.show();
    return a.exec();
}

