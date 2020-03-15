#!/usr/bin/python3
# coding=utf-8
import re
import sqlite3
import os

MD5_TABLE_MODE = 0
PE_MD5_TABLE_MODE = 1
STR_TABLE_MODE = 2

def tableNum(conn, table_name):
    c = conn.cursor()
    cursor = c.execute("select count(*) from sqlite_master where type='table' and name='{0}'".format(table_name))
    for row in cursor:
        return row[0]

def createTable(conn, tableName):
    if 0 == tableNum(conn, tableName):
        c = conn.cursor()
        c.execute('''CREATE TABLE {0}
            (ID INTEGER PRIMARY KEY AUTOINCREMENT,
             NAME TEXT    NOT NULL
            );'''.format(tableName))
        print("Table created successfully");

def createChildTable(conn, tableName, mode):
    if 0 != tableNum(conn, tableName):
        return 0

    c = conn.cursor()
    if MD5_TABLE_MODE == mode:
        c.execute('''CREATE TABLE {0} 
            (ID INTEGER NOT NULL,
            FILESIZE INTERGER,
            MD5 TEXT NOT NULL UNIQUE,
            FOREIGN KEY (ID) REFERENCES av_sig(ID) );'''.format(tableName))
    elif PE_MD5_TABLE_MODE == mode:
        c.execute('''CREATE TABLE {0} 
            (ID INTEGER NOT NULL,
            FILESIZE INTERGER,
            MD5 TEXT NOT NULL UNIQUE,
            FOREIGN KEY (ID) REFERENCES av_sig(ID) );'''.format(tableName))
    elif STR_TABLE_MODE == mode:
        c.execute('''CREATE TABLE {0} 
            (ID INTEGER NOT NULL,
            TYPE TEXT,
            OFFSET TEXT,
            STRING TEXT NOT NULL UNIQUE,
            FOREIGN KEY (ID) REFERENCES av_sig(ID) );'''.format(tableName))
    print("Table created successfully");

def insert(curs, tableName, key, value):
    #print("INSERT INTO {0} ({1}) VALUES ({2});".format(tableName, key, value))
    curs.execute("INSERT INTO {0} ({1}) VALUES ({2});".format(tableName, key, value))

def delete(curs, tableName, condition):
    curs.execute("DELETE FROM {0} WHERE {1};".format(tableName, condition))

def showAll(curs, tableName):
    cursor = curs.execute("SELECT * FROM {0};".format(tableName))
    for row in cursor:
        print(row[0:len(row)])

def last_insert_id(curs):
    cursor = curs.execute('select last_insert_rowid() from av_sig')
    values = cursor.fetchone()
    return values[0]

def readdb(conn, fileName):
    subName = os.path.splitext(fileName)[-1]
    curs = conn.cursor()
    file = open(fileName, "r")
    if ".hdb" == subName or ".hsb" == subName:
        for line in file:
            try:
                col = line.split(":")
                insert(curs, "av_sig", "NAME", "'{0}'".format(col[2]))
                avid = last_insert_id(curs)
                insert(curs, "av_sig_md5", "ID, FILESIZE, MD5", \
                       "'{0}', '{1}', '{2}'".format(avid, col[1], col[2]))
            except sqlite3.IntegrityError as e:
                print(e.args, "ID={0}".format(avid))

    elif ".mdb" == subName:
        for line in file:
            try:
                col = line.split(":")
                insert(curs, "av_sig", "NAME", "'{0}'".format(col[2]))
                avid = last_insert_id(curs)
                insert(curs, "av_sig_mdb", "ID, FILESIZE, MD5", \
                       "'{0}', '{1}', '{2}'".format(avid, col[0], col[1]))
            except sqlite3.IntegrityError as e:
                print(e.args, "ID={0}".format(avid))
    elif ".ndb" == subName:
        for line in file:
            col = line.split(":")
            try:
                insert(curs, "av_sig", "NAME", "'{0}'".format(col[0]))
                avid = last_insert_id(curs)
                insert(curs, "av_sig_str", "ID, TYPE, OFFSET, STRING", \
                       "'{0}', '{1}', '{2}', '{3}'".format(avid, col[1], col[2], col[3]))
            except sqlite3.IntegrityError as e:
                print(e.args, "ID={0}".format(avid))

def main():
    tableName = "av_sig"
    conn = sqlite3.connect('test.db')
    #创建表格
    createTable(conn, tableName)
    createChildTable(conn, "av_sig_md5", MD5_TABLE_MODE)
    createChildTable(conn, "av_sig_pe_md5", PE_MD5_TABLE_MODE)
    createChildTable(conn, "av_sig_str", STR_TABLE_MODE)
    #开启外键约束
    c = conn.cursor()
    c.execute("PRAGMA foreign_keys = ON;")

    readdb(conn, "main.hsb")
    #showAll(c, tableName)
    #showAll(c, "av_sig_md5")

    conn.commit()
    conn.close()

# 需要添加异常捕获机制
def test():
    tableName = "av_sig"
    conn = sqlite3.connect('test.db')
    createTable(conn, tableName)
    createChildTable(conn, "av_sig_md5", MD5_TABLE_MODE)
    curs = conn.cursor()
    try:
        insert(curs, "av_sig", "NAME", "'banana2'")
        avid = last_insert_id(curs)
        insert(curs, "av_sig_md5", "ID, FILESIZE, MD5", "{0},'10','f653cae'".format(avid))
    except sqlite3.IntegrityError as e:
        print(e.args)
        conn.rollback()
        #delete(curs, "av_sig", "ID={0}".format(avid))
        #print(last_insert_id(conn.cursor()))
        print(avid)
    else:
        print("insert success")

    print(last_insert_id(conn.cursor()))
    conn.commit()
    conn.close()

main()

















