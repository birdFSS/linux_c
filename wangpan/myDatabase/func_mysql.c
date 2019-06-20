#include <func.h>
#include <mysql/mysql.h>

int queryMySQL(MYSQL* conn, char* queryInfo, char* resultInfo);
int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo);


int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, insertInfo);
    if(ret){
        printf("Error making query:%s\n", mysql_error(conn));
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
    char queryDir[100]="select * from file";
    insertMySQL(conn,insertInfo, NULL);
    queryMySQL(conn,queryInfo,NULL);
    insertMySQL(conn, insertDir, NULL);
    queryMySQL(conn, queryDir, NULL);
    mysql_close(conn);
    return 0;
}
*/
