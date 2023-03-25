#!/bin/bash

#LD_LIBRARY_PATH=/home/zoli/commonlib_debug_64/:/home/zoli/Qt/5.9/gcc_64/lib:/home/zoli/Qt/5.9/gcc_64/lib
TEMPLATE=~/buildnumber.h.tmp
OUTPUTFILE=~/insole1/buildnumber.h
PROJECT=project_name

#LD_LIBRARY_PATH=$LD_LIBRARY_PATH
~/buildnum_ -t $TEMPLATE -o $OUTPUTFILE -p $PROJECT
