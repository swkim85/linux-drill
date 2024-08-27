// event.hip
// https://github.com/ROCm/HIP/issues/3383
#include <hip/hip_runtime.h>
#include <iostream>
#include <chrono>

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
  std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
  exit(1); \
  }

__global__
void sleep(uint64_t delta) {
  if (blockIdx.x == 0 and threadIdx.x == 0) {
    uint64_t start = clock64();
    while (clock64() - start < delta);
  }
}

int main (void) {

  // initialise the HIP runtime
  HIP_CHECK(hipFree(nullptr));

  // create a HIP stream
  hipStream_t stream;
  HIP_CHECK(hipStreamCreate(&stream));

  // create a HIP event
  hipEvent_t event;
  HIP_CHECK(hipEventCreate(&event));

  // check the event
  auto status = hipEventQuery(event);
  std::cout << "HIP event status: " << status << " (" << hipGetErrorName(status) << ")\n";

  // submit asynchronous work and record the event
  uint64_t rate_kHz = 1'700'000;  // 1700 MHz
  uint64_t time_ms = 10'000;      // 10 s
  sleep<<<1, 1, 0, stream>>>(time_ms * rate_kHz);
  HIP_CHECK(hipEventRecord(event, stream));

  // check the event
  status = hipEventQuery(event);
  std::cout << "HIP event status: " << status << " (" << hipGetErrorName(status) << ")\n";

  // wait for all asynchronous work to complete
  HIP_CHECK(hipEventSynchronize(event));
  std::cout << "asynchronous work complete\n";

  // check the event
  status = hipEventQuery(event);
  std::cout << "HIP event status: " << status << " (" << hipGetErrorName(status) << ")\n";

  // clean up
  HIP_CHECK(hipEventDestroy(event));
  HIP_CHECK(hipStreamDestroy(stream));
}
