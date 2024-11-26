// shm_write.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_SIZE 1024  // 공유 메모리 크기

int main() {
    key_t key = ftok("shmfile", 65); // 고유 키 생성
    printf("Generated IPC key: %d\n", key);

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // 공유 메모리 생성

    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *str = (char*)shmat(shmid, NULL, 0); // 공유 메모리 연결
    if (str == (char*)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Write Data : ");
    fgets(str, SHM_SIZE, stdin); // 사용자로부터 입력 받기
    printf("Data written in memory: %s\n", str);

    shmdt(str); // 공유 메모리 분리
    return 0;
}
