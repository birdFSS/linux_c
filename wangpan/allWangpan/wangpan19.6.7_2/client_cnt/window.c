#include "client.h"

static int userEnroll(int fd){
    linkMsg_t lmsg;
    bzero(&lmsg, sizeof(lmsg));
    
    printf("请输入您的用户名：(小于20个字符)\n");
    fflush(stdin);
    fgets(lmsg.buf, 20, stdin);
    lmsg.buf[strlen(lmsg.buf)-1] = '$';

    fflush(stdin);
    printf("请输入您的密码：(小于20个字符)\n");
    fgets(lmsg.buf+strlen(lmsg.buf), 20, stdin);
    lmsg.buf[strlen(lmsg.buf)-1] = '\0';

    //printf("test buf = %s##\n", lmsg.buf);
    
    lmsg.size = 16 + strlen(lmsg.buf);
    lmsg.flag = USERENROLL;
    send(fd, &lmsg, lmsg.size, 0);
    //为什么没有卡住?
    recvCycle(fd, &lmsg, 16);
    recvCycle(fd, lmsg.buf, lmsg.size - 16);
    if(FAIL_FLAG == lmsg.flag){
        printf("%s\n", lmsg.buf);
        return -1;
    }else{
        printf("注册成功！\n");
    }
    return 0;
}

static int userLogin(int fd){
    linkMsg_t lmsg;
    bzero(&lmsg, sizeof(lmsg));
    fflush(stdin);
    printf("请输入您的用户名：(小于20个字符)\n");
    fgets(lmsg.buf, 20, stdin);
    lmsg.buf[strlen(lmsg.buf)-1] = '\0';
    lmsg.flag = USERLOGIN;
    lmsg.size = strlen(lmsg.buf) + 16;
    send(fd , &lmsg, lmsg.size, 0);
    
    //接收返回信息，salt值或者错误值
    recvCycle(fd, &lmsg, 16);
    recvCycle(fd, lmsg.buf, lmsg.size);
    if(SUCCESS == lmsg.flag){
        char* passwd;
        passwd = getpass("请输入密码:");
        char* tempStr = crypt(passwd, lmsg.buf);
        bzero(lmsg.buf, sizeof(lmsg.buf));
        strcpy(lmsg.buf, tempStr);

        //将密文发回去给服务器
        lmsg.size = strlen(lmsg.buf) + 16;
        lmsg.flag = USERLOGIN;
        send(fd, &lmsg, lmsg.size, 0);
        recvCycle(fd, &lmsg, 16);
        memset(lmsg.buf, 0, sizeof(lmsg.buf));
        recvCycle(fd, lmsg.buf, lmsg.size);
        if(SUCCESS == lmsg.flag){
            printf("登录成功\n");
        }else{
            printf("error:%s\n", lmsg.buf);
            return -1;
        }
    }else{
        printf("error: %s\n", lmsg.buf);
        return -1;
    }

    return 0;
}

int windowForLogin(int fd){
    int i, ret;
    while(1){
        system("clear");
        for(i=0;i<80;i++){
            printf("*");
        }
        printf("\n1.登录\n2.注册账户\n3.退出\n");
        int choice;
        scanf("%d",&choice);
        getchar();
        if(1 == choice)
        {
            ret = userLogin(fd);            
            if(0 == ret)
            {
               break; 
            }
        }else if(2 == choice){
            userEnroll(fd);
        }else if(3 == choice){
            return -1;
        }
    }
    return 0;
}

