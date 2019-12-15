#!/bin/bash
mkdir $1
cd $1
mkdir src include obj tmp

strDate=$(date)
echo "project name $1"
echo "createDate $strDate" > README.md
echo "author     Fu Fenghai" >>README.md
