#ifndef BUILDNUMBER_H
#define BUILDNUMBER_H

#define BUILDNUMBER 1301

#include <QString>

struct Buildnumber{ static QString toString(); };

#endif // BUILDNUMBER_H

/*
// buildnumber.cpp should contain this lines below to forcing build with makefile depencencies
#include <QString>
#include "buildnumber.h"

QString Buildnumber::toString(){return QString::number(BUILDNUMBER);}
*/

//buildnum_insole.sh
/*
#!/bin/bash

LD_LIBRARY_PATH=/home/zoli/commonlib_debug_64/:/home/zoli/Qt/5.9/gcc_64/lib:/home/zoli/Qt/5.9/gcc_64/lib
TEMPLATE=~/buildnumber.h.tmp
OUTPUTFILE=~/insole1/buildnumber.h
PROJECT=pizero_mcp3008_api

LD_LIBRARY_PATH=$LD_LIBRARY_PATH ~/buildnum_ -t $TEMPLATE -o $OUTPUTFILE -p $PROJECT
*/

/*
.pro file

TARGET = PiCameraCV
message( "TARGET = "$$TARGET )
DEFINES += QT_TARGET='\\"$$TARGET\\"'
*/
