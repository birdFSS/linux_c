#!/bin/bash

mkdir -p ~/.vim/plugin/
mkdir -p ~/.vim/doc/

echo ">>>>>>>>>>>>>> install cscope-vim"
sudo apt install cscope
cp ./cscope_maps.vim ~/.vim/plugin/


echo ">>>>>>>>>>>>>> install YCM-generator"
cp ./YCM-Generator ~/.vim/ -rf


echo ">>>>>>>>>>>>> install taglist-vim"
git clone https://github.com/yegappan/taglist
cp ./taglist/plugin/taglist.vim ~/.vim/plugin/
cp ./taglist/doc/taglist.txt ~/.vim/doc/

rm -rf ./taglist

echo ">>>>>>>>>>>>> cp .vimrc"
cat vimrc_new >> ~/.vimrc


