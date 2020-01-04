#!/bin/bash
# ubuntu 18.04 CUnit安装全流程
echo 安装CUnit
echo 首先安装libtool automake
sudo apt install libtool
sudo apt install automake
mkdir tmp
cd tmp
wget https://nchc.dl.sourceforge.net/project/cunit/CUnit/2.1-3/CUnit-2.1-3.tar.bz2
tar xvf CUnit-2.1-3.tar.bz2
cd CUnit-2.1-3
echo 剩下参考另一篇博客
echo https://mp.csdn.net/postedit/103609751

aclocal
autoconf
autoheader
automake --add-missing
libtoolize --automake --copy --debug --force
./configure
automake --add-missing
make
sudo make install

ls /usr/local/lib/libcunit.so


