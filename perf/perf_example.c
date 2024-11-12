
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

#define PERF_COUNT_HW_CPU_CYCLES          0
#define PERF_COUNT_HW_CACHE_MISSES        3

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags)
{
    int ret;
    ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
    return ret;
}

int main(int argc, char **argv)
{
    struct perf_event_attr pe;
    long long count[2];
    int fd[2];

    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.type = PERF_TYPE_HARDWARE;
    pe.size = sizeof(struct perf_event_attr);
    pe.disabled = 1;
    pe.exclude_kernel = 1;
    pe.exclude_hv = 1;

    // CPU 사이클 이벤트 설정
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    fd[0] = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd[0] == -1) {
        fprintf(stderr, "Error opening CPU cycles event\n");
        exit(EXIT_FAILURE);
    }

    // 캐시 미스 이벤트 설정
    pe.config = PERF_COUNT_HW_CACHE_MISSES;
    fd[1] = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd[1] == -1) {
        fprintf(stderr, "Error opening cache misses event\n");
        exit(EXIT_FAILURE);
    }

    // 이벤트 활성화
    ioctl(fd[0], PERF_EVENT_IOC_RESET, 0);
    ioctl(fd[1], PERF_EVENT_IOC_RESET, 0);
    ioctl(fd[0], PERF_EVENT_IOC_ENABLE, 0);
    ioctl(fd[1], PERF_EVENT_IOC_ENABLE, 0);

    // 측정하고자 하는 코드 실행
    printf("Measuring performance...\n");
    sleep(1);

    // 이벤트 비활성화 및 결과 읽기
    ioctl(fd[0], PERF_EVENT_IOC_DISABLE, 0);
    ioctl(fd[1], PERF_EVENT_IOC_DISABLE, 0);
    read(fd[0], &count[0], sizeof(long long));
    read(fd[1], &count[1], sizeof(long long));

    printf("CPU cycles: %lld\n", count[0]);
    printf("Cache misses: %lld\n", count[1]);

    close(fd[0]);
    close(fd[1]);
    return 0;
}
