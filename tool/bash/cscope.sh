#!/bin/bash

wget http://cscope.sourceforge.net/cscope_maps.vim
mkdir -p ~/.vim/plugin/
mv cscope_maps.vim ~/.vim/plugin/

echo "cscope -R (create cscope.out)"
echo "CT + ] (function jump)"
echo "CT + \ ; s (find function all)"
echo "CT + t (jump back)"

