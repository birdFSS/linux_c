#include "client.h"

/*判断是否是字母*/
static int isAlpha(char c){
    if((c >= 'A' && c <= 'Z') || (c >='a' && c<='z')){
        return TRUE;
    }else{
        return FALSE;
    }
}

/*判断是否是空格或tab*/
static int isSpace(char c){
    if(' ' == c || '\t' == c){
        return TRUE;
    }else{
        return FALSE;
    }
}

/* from表示命令，to表示格式化以后的命令*/
static void dealLine(char* from, char* to){
    while('\0' != *from && '\n' != *from){
        while(*from == ' ' || *from == '\t'){   
            ++from;
        }   
        while(!isSpace(*from) && *from != '\0' && *from != '\n'){
            *to++ = *from++;
        }
        if(*from != '\0'){
            *to++ = ' ';
        }
    }
}

/*
 * 功能：对输入的字符处理识别，并返回对应命令值
 * 通过dataStr返回操作的目录，或上传的文件名，或下载文件名
*/
int getCommendFromStdin(char* dataStr){
    char commendStr[STDIN_MAX];
    char buf[STDIN_MAX], afterDealStr[STDIN_MAX];
    bzero(commendStr, STDIN_MAX);
    bzero(buf, STDIN_MAX);
    bzero(afterDealStr, STDIN_MAX);
    int commendNum = -1;
    fgets(buf, STDIN_MAX, stdin);
    buf[strlen(buf)-1] = '\0';
    fflush(stdin);
    int i=0;
    //对字符串先进行处理
    dealLine(buf, afterDealStr);
    char *p = afterDealStr;
    //取出指令
    while(*p != ' ' && *p != '\n'){
        commendStr[i++] = *p++;
    }
    p++;
    //取出操作对应的字符串
    strcpy(dataStr, p);
    //分析输入的命令
    if(!strcmp(commendStr, "cd")){
        commendNum = CDCOMMEND;
    }else if(!strcmp(commendStr, "ls")){
        commendNum = LSCOMMEND;
    }else if(!strcmp(commendStr, "gets")){
        commendNum = GETSCOMMEND;
    }else if(!strcmp(commendStr, "puts")){
        commendNum = PUTSCOMMEND;
    }else if(!strcmp(commendStr, "remove") || !strcmp(commendStr, "rm")){
        commendNum = RMCOMMEND;
    }else if(!strcmp(commendStr, "pwd")){
        commendNum = PWDCOMMEND;
    }else{
        printf("%s : commend not found\n", commendStr);
        goto ERROR_GOTO;
    }
    return commendNum;
ERROR_GOTO:
    return -1;
}
/*
int main(){
    char test[500] = {0};
    int ret = getCommendFromStdin(test);
    printf("ret = %d, data = %s\n", ret, test);
    return 0;
}
*/
