#!/bin/bash
mkdir $1
cd $1
mkdir src include obj tmp

strDate=$(date)
echo "createDate $strDate" > README.md
echo "author     Fu Fenghai" >>README.md
