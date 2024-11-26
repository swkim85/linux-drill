// shm_read.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024  // 공유 메모리 크기

int main() {
    key_t key = ftok("shmfile", 65); // 고유 키 생성
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT); // 공유 메모리 접근

    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    char *str = (char*)shmat(shmid, NULL, 0); // 공유 메모리 연결
    if (str == (char*)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Data read from memory: %s\n", str); // 공유 메모리에서 데이터 읽기

    shmdt(str); // 공유 메모리 분리
    // shmctl(shmid, IPC_RMID, NULL); // 공유 메모리 삭제
    return 0;
}
