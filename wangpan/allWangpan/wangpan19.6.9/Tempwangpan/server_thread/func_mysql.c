#include "factory.h"

int connectMYSQL(MYSQL** pConn){
    char server[20] = "localhost";
    char user[20] = "root";
    char password[20] = "891188";
    char database[20] = "user";
    *pConn = mysql_init(NULL);
    if(!mysql_real_connect(*pConn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s", mysql_error(*pConn));
        return -1;
    }else{
        printf("connect database success\n");
    }
    return 0;
}

int updateMySQL(MYSQL* conn, char* updateInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, updateInfo);
    if(ret)
    {
        sprintf(resultInfo, "%s:%s","Error making update", mysql_error(conn));
        return -1;
    }else{
        sprintf(resultInfo, "%s","update success");
    }
    return 0;
}

int deleteMySQL(MYSQL *conn, char* deleteInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, deleteInfo);
    if(ret)
    {
        sprintf(resultInfo, "%s:%s","Error making delete", mysql_error(conn));
        return -1;
    }else{
        sprintf(resultInfo, "%s","delete success");
    }
    return 0;
}

int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo){
    int ret;
    ret = mysql_query(conn, insertInfo);
    if(ret){
        printf("error in insertMysql\n");
        sprintf(resultInfo, "%s:%s","Error making insert", mysql_error(conn));
        return -1;
    }else{
        printf("error in insertMysql\n");
        sprintf(resultInfo, "%s","insert success");
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
        sprintf(resultInfo,"%s:%s","Error making query", mysql_error(conn));
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
                    sprintf(resultInfo,"%s %8s", resultInfo,row[queryNum]);
                }
            }
        }else{
            sprintf(resultInfo, "%s","Don't find data");
        }
        mysql_free_result(res);
    }
    return 0;
}
/*//测试案例
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
