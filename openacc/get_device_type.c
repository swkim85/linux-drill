// get_device_type.c
#include <openacc.h>
#include <stdio.h>

int main() {
  acc_device_t device_type = acc_get_device_type();
  printf("Current device type: %d\n", device_type);
  return 0;
}
