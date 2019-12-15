#!/bin/bash
mkdir $1
cd $1
mkdir src include obj tmp
cp ~/cLinux/myMakefile/Makefile .

strDate=$(date)
echo "project name $1" > README.md
echo "createDate $strDate" >> README.md
echo "author     Fu Fenghai" >>README.md
