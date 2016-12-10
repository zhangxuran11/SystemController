#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T14:49:52
#
#-------------------------------------------------

QT       += core gui network
QMAKE_LFLAGS += -unwind -g -ldl -rdynamic
QMAKE_CFLAGS += -unwind -g -rdynamic
TARGET = SystemController
TEMPLATE = app
CONFIG += qt debug warn_on
include(InputMethodPkg/InputMethod.pri)
include(qextserialport/src/qextserialport.pri)
include(BroadcastControllerPkg/BroadcastControllerPkg.pri)
include(ModbusManager/ModbusManager.pri)
include(ZTools/ZTools.pri)
include(ZDaoPkg/ZDaoPkg.pri)
include(GPS_Driver/GPS_Driver.pri)
include(GPSMapManager/GPSMapManager.pri)
include(Tools/Tools.pri)
include(ZTPManager/ZTPManager.pri)
include(TrainLineEditPkg/TrainLineEditPkg.pri)
include(GPIOPkg/GPIOPkg.pri)
include(ZDebugPkg/ZDebugPkg.pri)

DEFINES += $$(ARCH)
contains( DEFINES, arm ) {
    DEFINES += ARM
    mytarget.commands = scp ./${TARGET} root@192.168.1.10:/appbin/ \
}
!contains( DEFINES, arm ) {
    DEFINES += X86
}
SOURCES += main.cpp\
        mainwindow.cpp \
    workthread.cpp \
    globalinfo.cpp \
    softwatchdog.cpp \
    carinfo.cpp \
#    crashtool.cpp \
    iodevicelistener.cpp \
    zdownloadthread.cpp \
    versionsender.cpp \
    devinfo.cpp \
    espeakedit.cpp \
    vserverctrl.cpp

HEADERS  += mainwindow.h \
    config.h \
    workthread.h \
    globalinfo.h \
    softwatchdog.h \
    carinfo.h \
    crashtool.h \
    iodevicelistener.h \
    zdownloadthread.h \
    versionsender.h \
    devinfo.h \
    espeakedit.h \
    vserverctrl.h

FORMS    += mainwindow.ui \
    espeakedit.ui




mytarget.target = a

mytarget.depends =



QMAKE_EXTRA_TARGETS += mytarget

OTHER_FILES += \
    qss/mainframe.qss

RESOURCES += \
    resource.qrc \

DISTFILES +=
