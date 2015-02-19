
#include "kernel.h"

__global__ void reduce(int *in, int *out, int size) {

  // Create Shared Memory
  extern __shared__ int memPnt[];

  int tid = threadIdx.x;
  int index = blockIdx.x*blockDim.x*2 + threadIdx.x;
  int acc = 0;

  // Here is where striding is accomplished
  while(index < size){
    if( index < size){
      acc += in[index];
    }
    // First level reduction: Read from global into shared memory
    if(index + blockDim.x < size){
      acc += in[index+blockDim.x];
    }
    index += blockDim.x * gridDim.x;
  }

    memPnt[tid] = acc;
    __syncthreads();

  for(int j=blockDim.x/2; j > 0; j>>=1){
    if(tid < j){
      memPnt[tid] = acc = acc + memPnt[tid + j];
    }

    __syncthreads();
  }
  if(tid == 0)
    out[blockIdx.x] = acc;

}
