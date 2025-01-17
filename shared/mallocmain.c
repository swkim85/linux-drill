#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *abc;
    abc = (char *)malloc(8);
    printf("malloced address: %p\n", abc);
    sprintf(abc, "hello\n");
    printf("%s", abc);
    free(abc);
    return 0;
}

