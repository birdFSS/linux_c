#include <func.h>
#define  STR_LEN 8
void GenerateStr(char *str){
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

int main(int argc, char* argv[])
{
    char str[STR_LEN+1] = {0};
    GenerateStr(str);
    return 0;
}

