// mlimit.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256

long long get_memory_limit() {
  FILE *cgroup_file = fopen("/proc/self/cgroup", "r");
  if (!cgroup_file) {
    perror("Failed to open /proc/self/cgroup");
    return -1;
  }

  char line[MAX_LINE];
  char *cg_path = NULL;

  while (fgets(line, sizeof(line), cgroup_file)) {
    //printf("line = %s \n", line);
    char *path = strchr(line, ':');
    if (path && path[1] == ':') {
      cg_path = strdup(path+2);
      cg_path[strcspn(cg_path, "\n")] = 0;
      //printf("cg_path = %s \n", cg_path );
    }
  }
  fclose(cgroup_file);

  if (!cg_path) {
    fprintf(stderr, "cg_path not found\n");
    return -1;
  }

  char limit_file_path[MAX_LINE];
  snprintf(limit_file_path, sizeof(limit_file_path), "/sys/fs/cgroup%s/memory.max", cg_path);
  //free(cg_path);
  //printf("limit_file_path = %s \n", limit_file_path);

  FILE *limit_file = fopen(limit_file_path, "r");
  if (!limit_file) {
    perror("Failed to open memory limit file");
    return -1;
  }

  long long limit;
  if (fscanf(limit_file, "%lld", &limit) != 1) {
    fprintf(stderr, "Failed to read memory limit\n");
    fclose(limit_file);
    return -1;
  }
  fclose(limit_file);

  return limit;
}

int main() {
  long long memory_limit = get_memory_limit();
  if (memory_limit != -1) {
    printf("Memory limit: %lld bytes\n", memory_limit);
    printf("Memory limit: %.2f MB\n", (double)memory_limit / (1024 * 1024));
  }
  return 0;
}
