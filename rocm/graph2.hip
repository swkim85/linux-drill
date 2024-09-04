// graph2.hip
#include <hip/hip_runtime.h>
#include <rocrand/rocrand_kernel.h>
#include <iostream>
#include <chrono>

#define MAX_NODES 1024  // 최대 처리가능한 노드의수

#define HIP_CHECK(status) \
  if (status != hipSuccess) { \
    std::cerr << "HIP error: " << hipGetErrorString(status) << std::endl; \
    exit(1); \
  }

struct MyStruct {   // 스레드 내부의 인덱스 값을 저장
  int idx;
  int thread_id;
  int block_id;
  int blockdim_id;
  int first_visit;
  int last_visit;
  int length;
  rocrand_state_xorwow  rng; // random number generator
};

__managed__  unsigned long long   seed;  // 난수 발생기 seed

__device__ int get_next_adj_node( int curr_node, int *adj, int n, int *visited ) {
  for (int i = 0; i < n; i++) {
    int index = curr_node * n + i; // i: next node
    if (!adj[index]) continue; // 인접하지 않음
    if (!visited[i]) { // 아직 방문하지 않음
      return i;
    }
  }
  return -1; // 더이상 방문할 노드가 없음
}

__device__ int get_next_random_adj_node( MyStruct *kinfo, int idx, int curr_node, int *adj, int n, int *visited ) {
  int stack_data[MAX_NODES]; // stack
  int stack_top = 0;

  for (int i = 0; i < n; i++) {
    int index = curr_node * n + i; // i: next node
    if (!adj[index]) continue; // 인접하지 않음
    if (!visited[i]) { // 아직 방문하지 않음
      stack_data[stack_top++] = i; // 다음에 방문할 인접 노드들을 stack에 추가
    }
  }
  if (stack_top == 0) return -1; // 더이상 방문할 노드가 없음

  // stack에서 랜덤하게 하나를 골라서 방문
  int rand = floor(rocrand_uniform (&(kinfo[idx].rng)) * stack_top);
  return stack_data[rand];
}


// adj : 그래프 인접 노드 정보
// n : 노드 개수
__global__ void myKernel(MyStruct *kinfo, int *adj, int n)
{
  int idx = threadIdx.x + blockIdx.x * blockDim.x;
  if (idx >= n) return;

  kinfo[idx].idx = idx;  // 현재 스레드의 정보를 저장 
  kinfo[idx].thread_id   = threadIdx.x;
  kinfo[idx].block_id    = blockIdx.x;
  kinfo[idx].blockdim_id = blockDim.x;
  kinfo[idx].first_visit = -1;
  kinfo[idx].last_visit = -1;
  kinfo[idx].length = 0;

  rocrand_init(seed, idx, 0, &(kinfo[idx].rng)); // XORWOW 상태 초기화

  int from_node = idx; // idx 값에 해당하는 노드에서 시작
  int currentLength = 0;

  int visited[MAX_NODES];
  for (int i = 0; i < n; i++) visited[i] = 0; // 초기화

  int curr_node = from_node;
  visited[curr_node] = true;
  while (1) {
    //int next = get_next_adj_node( curr_node, adj, n, visited );
    int next = get_next_random_adj_node( kinfo, idx, curr_node, adj, n, visited );
    if (next == -1) break; // 더 방문할 노드가 없으면 종료

    if (kinfo[idx].first_visit == -1) kinfo[idx].first_visit = next;
    visited[next] = true;
    kinfo[idx].last_visit = next;
    currentLength++;
    curr_node = next;

    //if (currentLength == 2) break;
  }
  kinfo[idx].length = currentLength;

}

void print_matrix(std::string title, int *data, int rows, int cols) {
  if (title.length()) std::cout << title << std::endl;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      std::cout.width(4);
      std::cout << data[i*rows + j] << "  ";
    }
    std::cout << std::endl;
  }
}

int main() {

  int n, m; // n: number of vertices, m: number of edges
  std::cin >> n >> m;

  int *adj;
  adj = (int *)malloc(n * n *sizeof(int)); // 호스트 메모리. node간의 연결을 표시. 2D matrix
  for (int i = 0; i < n*n; i++) adj[i] = 0; // 초기화

  for (int i = 0; i < m; ++i) { // edge 정보를 입력받음
    int u, v;
    std::cin >> u >> v;
    int a = u * n + v;  // v row, u column
    int b = v * n + u;  // u row, v column
    adj[a] = 1;
    adj[b] = 1;
  }
  print_matrix("adj matrix", adj, n, n);

  // 현재 시간을 seed로 사용
  seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

  int *d_adj;
  HIP_CHECK(hipMalloc(&d_adj, n*n*sizeof(int))); // 디바이스 메모리 할당
  HIP_CHECK(hipMemcpy(d_adj, adj, n*n*sizeof(int), hipMemcpyHostToDevice)); // 디바이스 메모리로 복사

  MyStruct *d_krnInfo, *h_krnInfo;
  h_krnInfo = new MyStruct[n];
  int krninfo_size = n * sizeof(struct MyStruct);
  hipMalloc((void**)&d_krnInfo, krninfo_size); // 디바이스 메모리 할당(kernel info)

  // Kernel 실행
  constexpr unsigned int block_size = 256; // 블록당 스레드 개수
  unsigned int grid_size = (n + block_size - 1) / block_size; // 블럭 개수
  myKernel<<<grid_size, block_size>>>(d_krnInfo, d_adj, n);
  hipDeviceSynchronize();


  hipMemcpy(h_krnInfo, d_krnInfo, krninfo_size, hipMemcpyDeviceToHost); // 호스트로 복사
  std::cout << "idx  .first_visit .last_visit  .length" << std::endl;
  for (int i = 0; i < n; i++) {
    std::cout << ""
       << "" << h_krnInfo[i].idx
       << "  " << h_krnInfo[i].first_visit
       << "  " << h_krnInfo[i].last_visit
       << "  " << h_krnInfo[i].length
       << std::endl;
  }

  hipFree(d_krnInfo);
  hipFree(d_adj);
  free(adj);
  return 0;
}
