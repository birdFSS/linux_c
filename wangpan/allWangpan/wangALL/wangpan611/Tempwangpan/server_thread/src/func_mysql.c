#include "../include/factory.h"


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
        //printf("error in insertMysql\n");
        sprintf(resultInfo, "%s:%s","Error making insert", mysql_error(conn));
        return -1;
    }else{
        //printf("123\n");
        sprintf(resultInfo, "%s","insert success");
    }
    return 0;
}

int queryMySQL(MYSQL* conn, char* queryInfo, char* resultInfo){
    int ret;
    unsigned queryNum;
    MYSQL_ROW row;
    MYSQL_RES *res;

    ret = mysql_query(conn,queryInfo);
    if(ret)
    {
        sprintf(resultInfo,"%s:%s","Error making query", mysql_error(conn));
        return -1;
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
                    sprintf(resultInfo,"%s %s", resultInfo,row[queryNum]);
                }
            }
        }else{
            sprintf(resultInfo, "%s","Don't find data");
            return -1;
        }
        mysql_free_result(res);
    }
    return 0;
}

int queryMySQLForUser(MYSQL* conn, char* queryInfo, User_t* pUser){
    int ret;
    MYSQL_ROW row;
    MYSQL_RES *res;

    ret = mysql_query(conn,queryInfo);
    if(ret)
    {
        return -1;
    }else{
        res = mysql_store_result(conn);
        if(res)
        {   
            if((row = mysql_fetch_row(res)) != NULL)
            {
                strcpy(pUser->id, row[0]);
                strcpy(pUser->name, row[1]);
                strcpy(pUser->salt, row[2]);
                strcpy(pUser->passwd, row[3]);
                strcpy(pUser->dirID, row[4]);
            }else{
                return -1;
            }
        }else{
            mysql_free_result(res);
            return -1;
        }
        mysql_free_result(res);
    }
    return 0;
}

int updateOPTime(MYSQL* conn, char* userID, time_t t){
    char updateUser[100]="update user set lastOPTime=";
    sprintf(updateUser, "%s%ld %s%s", updateUser, t, "where id=", userID);

    //printf("updateUser=  %s\n", updateUser);
    int ret;
    ret = mysql_query(conn, updateUser);

#ifdef DEBUG_SERVER
    if(ret)
    {
        printf("error updateOPTime\n");
        return -1;
    }else{
        printf("success time update\n");
    }
#endif

    return 0;
}

/*成功返回0，未能匹配或者超时返回-1*/
int checkToken(MYSQL* conn, char* token,int outTime)
{
    printf("in checkToken1\n");
    time_t tNew;
    tNew = time(NULL);
    char queryToken[200] = "select lastOPTime,id from user where token=";
    sprintf(queryToken, "%s'%s'", queryToken, token);
    int ret;
    char lastOPTimeAndId[100] = {0};
    ret = queryMySQL(conn, queryToken, lastOPTimeAndId);
    if(-1 == ret)
    {
        printf("dont find the token\n");
        return -1;
    }
    printf("in checkToken2\n");
    char lastOPTime[50] = {0};
    char userId[20] = {0};
    sscanf(lastOPTimeAndId, "%s %s", lastOPTime, userId);
    time_t lastTime = atol(lastOPTime);
    if(tNew - lastTime > outTime)
    {
        printf("out of time\n");
        return -1;
    }
    updateOPTime(conn, userId, tNew);
    printf("in checkToken3\n");
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
