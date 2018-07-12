#include <stdio.h>
int main(void)
{
    unsigned int x = 3, y = -1, z = 2;
    if (x > y)
        if (y < 0)
            z = 0;
        else
            z += 1;
    printf("%d\n", z);
    return 0;
}


