#include <func.h>
#include <assert.h>

typedef unsigned char* byte_pointer;

void show_bytes(byte_pointer start, int len)
{
    int i;
    for(i=0; i < len; ++i)
    {
        printf("%.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x)
{
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void* x)
{
    show_bytes((byte_pointer)&x, sizeof(void *));
}

void test_show_bytes(int val)
{
    int ival = val;
    float fval = (float)ival;
    int *pval = &val;
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
}

void test_big_min()
{
    int val = 0x87654321;
    byte_pointer valp = (byte_pointer)&val;
    show_bytes(valp, 1);
    show_bytes(valp, 2);
    show_bytes(valp, 3);
}

void test_string()
{
    const char* s = "abcdef";
    show_bytes((byte_pointer)s, strlen(s));
}

void test_bit()
{
    int x = 0x87654321;
    printf("x=%x\n", x);
    show_int(x);
    int a = x & 0xff;
    assert(a == 0x00000021);
    int b = x ^ ~0xff;
    assert(b == 0x789ABC21);
    int c = x | 0xff;
    show_bytes((byte_pointer)&c, sizeof(int));
    assert(c == 0x876543ff);
}


int main(int argc, char* argv[])
{
    test_bit();
    //test_show_bytes(12345);
    return 0;
}

