// mutex1.c
#include <pthread.h>
#include "errors.h"

typedef struct my_struct_tag {
    pthread_mutex_t     mutex;  /* Protects access to value */
    int                 value;  /* Access protected by mutex */
} my_struct_t;

my_struct_t data = {PTHREAD_MUTEX_INITIALIZER, 0};

int main (int argc, char *argv[])
{
    return 0;
}

