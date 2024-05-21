QT += network sql
QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += TARGI=$$TARGET
message( "TARGET = "$$TARGET )

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:HOME = $$system(echo $HOME)
win32:HOME = $$system(echo %userprofile%)

CONFIG(debug, debug|release){
BUILD = debug
} else {
BUILD = release
}

COMMON_LIBS = commonlib

equals(BUILD,debug) {
    #message( "build is _ debug" )
    COMMON_LIBS = $$COMMON_LIBS"_debug"
}

equals(BUILD,release){
    COMMON_LIBS = $$COMMON_LIBS"_release"
}

equals(QT_ARCH, x86_64){
    COMMON_LIBS = $$COMMON_LIBS"_64"
}

equals(QT_ARCH, x86){
    COMMON_LIBS = $$COMMON_LIBS"_32"
}

equals(QT_ARCH, arm){
    COMMON_LIBS = $$COMMON_LIBS"_arm"
}

message( "architecture = "$$QT_ARCH )
message( "commonlibs folder = "$$COMMON_LIBS )

#unix:HOME = $$system(echo $HOME)
#win32:HOME = $$system(echo %userprofile%)

# INSTALLDIR = $$COMMON_LIBS
COMMON_LIBS_FULLPATH = $$shell_path($$HOME/$$COMMON_LIBS)

message( "commonlibs full path = "$$COMMON_LIBS_FULLPATH)

contains(QMAKESPEC,.*linux-rasp-pi\d*-.*){
    message(rpi)
    CONFIG += rpi
}

#unix:!macx: LIBS += -L/home/zoli/build-common-Desktop_Qt_5_12_2_GCC_64bit2-Debug/stringhelper/ -lstringhelper
unix:!macx:
{
#message(LIBS added for unix:!macx)
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -llogger
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lshortguid # loggerhez kell
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lsignalhelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lCommandLineParserHelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lcoreappworker
##LIBS += -L$$COMMON_LIBS_FULLPATH/ -lstringhelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -ltextfilehelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lfilehelper
##LIBS += -L$$COMMON_LIBS_FULLPATH/ -ldownloadhelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lprocesshelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lmacrofactory

#LIBS += -L$$COMMON_LIBS_FULLPATH/ -linihelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lsettingshelper
#LIBS += -L$$COMMON_LIBS_FULLPATH/ -lxmlhelper
}

# unix:QMAKE_RPATHDIR += /usr/mylib
# QMAKE_LFLAGS += -Wl,-rpath, "$$system_path($$HOME/$$INSTALLDIR)"
#QMAKE_LFLAGS += -Wl,-rpath,"$$HOME/$$COMMON_LIBS_FULLPATH"
#QMAKE_LFLAGS += -Wl,-rpath,"/$$HOME/$$INSTALLDIR/macrofactory"
#QMAKE_LFLAGS += -Wl,-rpath,"/home/zoli/build-common-Desktop_Qt_5_9_0_GCC_64bit-Debug/stringhelper"
#QMAKE_LFLAGS += -Wl,-rpath,"/home/zoli/build-common-Desktop_Qt_5_9_0_GCC_64bit-Debug/macrofactory"
#QMAKE_LFLAGS += -Wl,-rpath,"/home/zoli/build-common-Desktop_Qt_5_12_2_GCC_64bit2-Debug/stringhelper"
#QMAKE_LFLAGS += -Wl,-rpath,"/home/zoli/build-common-Desktop_Qt_5_12_2_GCC_64bit2-Debug/macrofactory"
#INCLUDEPATH += $$HOME/common
#INCLUDEPATH += $$HOME/common
#DEPENDPATH += $$HOME/common

SOURCES += \
        environment.cpp \
        helpers/buildnumber.cpp \
        helpers/commandlineparserhelper.cpp \
        helpers/coreappworker.cpp \
        helpers/logger.cpp \
        helpers/processhelper.cpp \
        helpers/textfilehelper.cpp \
        main.cpp \
        helpers/networkhelper.cpp \
        settings.cpp \
        helpers/sqlhelper.cpp \
        helpers/signalhelper.cpp \
        work1.cpp

HEADERS += \
    buildnumber.h \
    environment.h \
    helpers/commandlineparserhelper.h \
    helpers/coreappworker.h \
    helpers/logger.h \
    helpers/networkhelper.h \
    helpers/processhelper.h \
    helpers/stringify.h \
    helpers/textfilehelper.h \
    settings.h \
    helpers/sqlhelper.h \
    helpers/signalhelper.h \
    work1.h


unix:rpi:{
message(LIBS added for raspberry_pi)
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:rpi: target.path = /home/pi/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    buildnum.sh \
    buildnumber.h.tmp

