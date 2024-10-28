#include <iostream>
#include <vector>
#include <math.h>

// function to add the elements of two arrays
__global__
void add(int n, float *x, float *y)
{
  int index = threadIdx.x;
  int stride = blockDim.x;
  for (int i = index; i < n; i+=stride)
      y[i] = x[i] + y[i];
}

int main(void)
{
  int N = 1<<20; // 1M elements
  //std::vector<float> x(N);
  //std::vector<float> y(N);

  float *x, *y;

  //Allocate unified memory- accessible from cpu or gpu

  cudaError_t errx = cudaMallocManaged(&x, N*sizeof(float));
  cudaError_t erry = cudaMallocManaged(&y, N*sizeof(float));
  if (errx != cudaSuccess || erry != cudaSuccess) {
    fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(errx));
    fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(erry));
  }
  // initialize x and y arrays on the host
  //for (auto& xi : x) xi = 1.0f;
  //for (auto& yi : y) yi = 2.0f;
 
  for (int i = 0; i < N; i++) {
    x[i] = 1.0f;
    y[i] = 2.0f;
  }

  int blockSize = 256;
  int numBlocks = (N + blockSize -1) / blockSize;
  // Run kernel on 1M elements on the GPU
  add<<<numBlocks,blockSize>>>(N,x,y);//(N, x.data(), y.data());

  cudaDeviceSynchronize();

  // Check for errors (all values should be 3.0f)
  float maxError = 0.0f;
  for (int i = 0; i < N; i++)//(auto yi : y)
    maxError = fmax(maxError, fabs(y[i]-3.0f)); //yi
  std::cout << "Max error: " << maxError << std::endl;
  
  // free memory
  cudaFree(x);
  cudaFree(y);
  return 0;
}
