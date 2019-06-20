#include "../include/client.h"



int connectMYSQL(MYSQL** pConn)
{
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

int updateMySQL(MYSQL* conn, char* updateInfo, char* resultInfo)
{
    int ret;
    ret = mysql_query(conn, updateInfo);
    if(resultInfo != NULL)
    {
        if(ret)
        {
            sprintf(resultInfo, "%s:%s","Error making update", mysql_error(conn));
            return -1;
        }else{
            sprintf(resultInfo, "%s","update success");
        }
    }
    return 0;
}

int deleteMySQL(MYSQL *conn, char* deleteInfo, char* resultInfo)
{
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

int insertMySQL(MYSQL* conn, char* insertInfo, char* resultInfo)
{
    int ret;
    ret = mysql_query(conn, insertInfo);
    if(ret){
        //printf("error in insertMysql\n");
        sprintf(resultInfo, "%s:%s","Error making insert", mysql_error(conn));
        return -1;
    }else{
        //printf("123\n");
        sprintf(resultInfo, "%s","insert success");
    }
    return 0;
}

int queryMySQL(MYSQL* conn, char* queryInfo, char* resultInfo)
{
    int ret;
    unsigned queryNum;
    MYSQL_ROW row;
    MYSQL_RES *res;
    int flag = -2;

    ret = mysql_query(conn,queryInfo);
    if(ret)
    {
        sprintf(resultInfo,"%s:%s","Error making query", mysql_error(conn));
        return -1;
    }else{
        res = mysql_store_result(conn);
        if(res)
        {
            //无论有没有结果都会进来这里，所以不能这样判断
            while((row = mysql_fetch_row(res)) != NULL)
            {
                //printf("line = %d\n", __LINE__);
                for(queryNum=0;queryNum<mysql_num_fields(res);queryNum++)
                {
                    sprintf(resultInfo,"%s %s", resultInfo,row[queryNum]);
                }
                flag = 0;
            }
        }else{
            sprintf(resultInfo, "%s","Don't find data");
            return -1;
        }
        mysql_free_result(res);
    }
    return flag;
}
