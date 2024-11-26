
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>

int main() {
    key_t key;
    const char *path = "/tmp/somefile"; // 존재하는 파일 경로
    int id = 'A'; // 프로젝트 식별자

    // ftok() 호출하여 IPC 키 생성
    key = ftok(path, id);
    if (key == (key_t)-1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    printf("Generated IPC key: %d\n", key);
    return 0;
}
