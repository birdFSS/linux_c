#!/bin/bash
# author : birdFSS
# version : 1.0.0                                                           
# date : 2019/12/20
# brief : 下载病毒特征库，并自动计算md5

today=$(date +%Y%m%d)
mainDir=/home/${USER}/virusDataBase/

if [ ! -d ${mainDir} ]
then
	mkdir ${mainDir}
fi

if [ ! -d ${mainDir}${today} ]
then
	mkdir ${mainDir}${today}
fi

dataBaseRoot1=http://database.clamav.net/main.cvd
dataBaseFile1=${mainDir}${today}/main.cvd
dataBaseRoot2=http://database.clamav.net/daily.cvd
dataBaseFile2=${mainDir}${today}/daily.cvd

wget $dataBaseRoot1 -O $dataBaseFile1
wget $dataBaseRoot2 -O $dataBaseFile2

cntFile1=main.txt
cntFile2=daily.txt
md5sum $dataBaseFile1 >> ${mainDir}${cntFile1}
md5sum $dataBaseFile2 >> ${mainDir}${cntFile2}

