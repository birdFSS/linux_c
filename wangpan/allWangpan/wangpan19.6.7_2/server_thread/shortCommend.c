#include "factory.h"
#define STR_LEN 10

static void GenerateStr(char *str){
    int i, flag;
    srand(time(NULL));
    for(i=0;i<STR_LEN;i++){
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break;
        case 2:
            str[i] = rand()%10 + '0';
            break;
        }
    }
    printf("%s\n", str);    
}

static int apartUserInformation(char* buf, User_t* pUser){
    char* p=buf;
    int i=0;
    while(*p != '$')
    {
        pUser->name[i++] = *p;
        p++;
    }
    p++;
    i=0;
    while(*p != '$' && *p != '\0')
    {
        pUser->passwd[i++] = *p;
        p++;
    }
    return 0;
}

int userEnroll(int fd, MYSQL* conn,linkMsg_t* plmsg){
    User_t user;
    int ret;
    bzero(&user, sizeof(User_t));
    apartUserInformation(plmsg->buf, &user);
    
    GenerateStr(user.salt);
    strcpy(user.Ciphertext, crypt(user.passwd, user.salt));
    //send(fd,user.salt, strlen(user.salt),0);
    char insertUser[300] = {0};
    char insertHead[50]  = "INSERT INTO user(name, salt, password) values(";
    sprintf(insertUser,"%s'%s','%s','%s')",insertHead,user.name, user.salt, user.Ciphertext);

    printf("insertUser=%s\n", insertUser);
    //清空信息，保存返回信息
    bzero(plmsg->buf,sizeof(plmsg->buf));
    ret = insertMySQL(conn,insertUser,plmsg->buf);
    printf("insertMy?/\n");
    if(-1 == ret)
    {
        printf("insert fail\n");
        plmsg->flag = FAIL_FLAG;
        plmsg->size = 16 + strlen(plmsg->buf);
        send(fd, plmsg, plmsg->size,0);
        return -1;
    }
    printf("insert success\n");
    plmsg->flag = SUCCESS;
    plmsg->size = 16 + strlen(plmsg->buf);
    send(fd, plmsg, plmsg->size,0); 
    return 0;
}

