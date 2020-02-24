
#include <stdio.h>
int sum (int, int);

struct point{
    char c;
    int a;
    int b;
};

int main()
{
    printf("%ld\n", sizeof(struct point));
    int sum1[10] = {0};
    int x = 10; 
    int y = 20;
    x = x + y;
    sum1[0] = x;
    sum1[1] = y;
    sum1[2] = x + y;
    printf("sumi %d\n", sum1[1]);

    return sum(x, y);
}

