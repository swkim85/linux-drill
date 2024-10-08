// dlmain.c
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
int main(int argc, char **argv) {
    void *handle;
    double (*cosine)(double);
    char *error;

    handle = dlopen ("libm.so.6", RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);  exit(1);
    }
    cosine = dlsym(handle, "cos");
    if ((error = dlerror()) != NULL)  {
        fputs(error, stderr); exit(1);
    }
    printf ("%f\n", (*cosine)(3.14));   // cos(PI) ==> -1
    sleep(60);
    dlclose(handle);
}
