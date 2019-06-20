#include <func.h>
#include <shadow.h>

void get_salt(char* salt, char* passwd){
    int i,j;
    for(i=0,j=0;passwd[i] && j != 3; ++i){
        if(passwd[i] == '$')
            ++j;
    }
    strncpy(salt, passwd, i-1);
}
int main(int argc, char* argv[])
{
    struct spwd *sp;
    char* passwd;
    char salt[512] = {0};
    if(argc != 2){
        printf("user_name\n");
        return -1;
    }

    passwd = getpass("请输入密码：");
    printf("passwd = %s", passwd);

    if((sp = getspnam(argv[1])) == NULL){
        printf("密码错误");
        return -1;
    }

    get_salt(salt, sp->sp_pwdp);
    printf("salt = %s", salt);
    char* p;
    if(strcmp(sp->sp_pwdp, crypt(passwd, salt))==0){
        printf("success\n");
        printf("%s\n", sp->sp_pwdp);
        p = crypt(passwd, salt);
        printf("p = %s\n",p);
        printf("%ld\n", strlen(sp->sp_pwdp));
    }else{
        printf("fail\n");
        printf("%s\n", sp->sp_pwdp);
        printf("%s\n", crypt(passwd,salt));
    }
    return 0;
}

