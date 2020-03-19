#!/usr/bin/env python
# coding=utf-8
#!/usr/bin/python
# -*- coding: utf-8 -*-
# 遍历所有文件夹，将指定格式文件，批量另存为其他文件(或其他格式)
import os
import re
import sys

def all_path(dirname, filter):
    result = []#所有的文件
    path = os.path.abspath(dirname)
    for maindir, subdir, file_name_list in os.walk(dirname):
        for filename in file_name_list:
            apath = os.path.join(maindir, filename)#合并成一个完整路径
            portion = os.path.splitext(apath)
            ext = portion[-1]  # 获取文件后缀 [0]获取的是除了文件名以外的内容
            if ext in filter:
                result.append(apath)
    return result

filter = [".hsb", ".hdb", ".mdb", ".msb", ".fp", ".sfp"] #设置过滤后的文件类型 当然可以设置多个类型
result = all_path(sys.argv[1], filter)
for file in result:
    print(file)
