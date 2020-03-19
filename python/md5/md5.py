#!/usr/bin/env python
# coding=utf-8
import hashlib

def get_file_md5(file_path):
    with open(file_path, 'rb') as f:
        md5obj = hashlib.md5()
        md5obj.update(f.read())
        _hash = md5obj.hexdigest()
        return str(_hash).upper()

print(get_file_md5("./md5.py"))
