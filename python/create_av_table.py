#!/usr/bin/python3
# coding=utf-8
import re
import sqlite3

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

def createChildTable(conn, tableName):
    if 0 == tableNum(conn, tableName):
        c = conn.cursor()
        c.execute('''CREATE TABLE {0} 
            (ID INTEGER NOT NULL,
             MD5 TEXT NOT NULL UNIQUE,
             FOREIGN KEY (ID) REFERENCES av_sig(ID) );'''.format(tableName))
        print("Table created successfully");

def insert(curs, tableName, key, value):
    curs.execute("INSERT INTO {0} ({1}) VALUES ({2})".format(tableName, key, value));

def showAll(curs, tableName):
    cursor = curs.execute("SELECT * FROM {0}".format(tableName))
    for row in cursor:
        print(row[0:len(row)])

tableName = "av_sig"

conn = sqlite3.connect('test.db')
createTable(conn, tableName)
createChildTable(conn, "av_sig_md5")
#开启外键约束
c = conn.cursor()
c.execute("PRAGMA foreign_keys = ON;")

insert(conn.cursor(), tableName,"name", "'apple'")
showAll(c, tableName)
showAll(c, "av_sig_md5")

conn.commit()
conn.close()
