#include <stdlib.h>

static int isdigit(int c)
{
    return ((c >= '0') && (c <= '9'));
}

int atoi(const char *num)
{
    int value = 0;

    while (*num && isdigit(*num))
        value = value * 10 + *num++  - '0';

    return value;
}
