#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    const char *text = "Hello, World!\n";
    ssize_t bytes_written;

    bytes_written = write(1, text, strlen(text));
    if (bytes_written == -1) return 1;
    return 0;
}

