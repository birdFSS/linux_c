#include "factory.h"


int updateMySQL(MYSQL* conn, char* updateInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, updateInfo);
    if(ret)
    {
        printf("Error making update:%s\n", mysql_error(conn));
        return -1;
    }else{
        printf("update success\n");
    }
    return 0;
}

int deleteMySQL(MYSQL *conn, char* deleteInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, deleteInfo);
    if(ret)
    {
        printf("Error making delete:%s\n", mysql_error(conn));
        return -1;
    }else{
        printf("delete success,delete row = %ld\n", (long)mysql_affected_rows(conn));
    }
    return 0;
}

int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, insertInfo);
    if(ret){
        printf("Error making insert:%s\n", mysql_error(conn));
        return -1;
    }else{
        printf("insert success\n");
    }
    return 0;
}

int queryMySQL(MYSQL* conn, char* queryInfo, char* resultInfo){
    int ret;
    unsigned queryNum;
    MYSQL_ROW row;
    MYSQL_RES *res;
    /*演示：如果不free，查询不到结果
    t = mysql_query(conn, queryInfo);
    if(t){
        printf("Error making query:%s\n", mysql_error(conn));
    }else{
        printf("query success\n");
    }
    res = mysql_use_result(conn);
    mysql_free_result(res);
    */
    ret = mysql_query(conn,queryInfo);

    if(ret)
    {
        printf("Error making query:%s\n", mysql_error(conn));
    }else{
        res = mysql_store_result(conn);
        if(res)
        {
            //printf("use result\n");
            while((row = mysql_fetch_row(res)) != NULL)
            {
                //printf("line = %d\n", __LINE__);
                for(queryNum=0;queryNum<mysql_num_fields(res);queryNum++)
                {
                    printf("%8s", row[queryNum]);
                }
                printf("\n");
            }
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);
    }
    return 0;
}
/*
int main(int argc, char* argv[])
{
    MYSQL *conn;
    char server[20] = "localhost";
    char user[20] = "root";
    char password[20] = "891188";
    char database[20] = "user";
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n", mysql_error(conn));
    }else{
        printf("connected...\n");
    }
    char queryInfo[100]="select * from user";
    char insertInfo[100]="Insert into user(name,salt,password,dirID) values('ffy',0,123,NULL)";
    char insertDir[100]="insert into file(prevID, userName, type, useNum) values(-1, 'ffh', 'd', 1)";
    char queryfile[100]="select * from file";
    insertMySQL(conn,insertInfo, NULL);
    queryMySQL(conn,queryInfo,NULL);
    insertMySQL(conn, insertDir, NULL);
    queryMySQL(conn, queryDir, NULL);
    char updateUser[100]="update user set dirID=1 where id=1";
    updateMySQL(conn,updateUser, NULL);
    char deleteUser[100] = "delete from user where id = 2";
    deleteMySQL(conn, deleteUser, NULL);
    mysql_close(conn);
    return 0;
}
*/
