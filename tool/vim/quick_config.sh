#!/bin/bash

mkdir -p ~/.vim/plugin/
mkdir -p ~/.vim/doc/

echo ">>>>>>>>>>>>>> install cscope-vim"
sudo apt install cscope
cp ./cscope_maps.vim ~/.vim/plugin/
cp ./YCM-Generator/fake-toolchain ~/.vim/ -rf
cp ./YCM-Generator/config_gen.py ~/.vim/


echo ">>>>>>>>>>>>> install taglist-vim"
git clone https://github.com/yegappan/taglist
cp ./taglist/plugin/taglist.vim ~/.vim/plugin/
cp ./taglist/doc/taglist.txt ~/.vim/doc/
rm -rf ./taglist

echo ">>>>>>>>>>>>> cp .vimrc"
cat vimrc_new >> ~/.vimrc


