#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[20] = "localhost";
    char user[20] = "root";
    char password[20] = "891188";
    char database[20] = "mydb2";
    char query[200] ="insert into class(classNum, teacherID, studentID) values(2,1,2)";
    int t,r;
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn, server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n", mysql_error(conn));
    }else{
        printf("Connected...\n");
    }
    t = mysql_query(conn,query);
    if(t)
    {
        printf("Error making query:%s\n", mysql_error(conn));
    }else{
        printf("insert success\n");
    }
    mysql_close(conn);
    return 0;
}

