
#include "kernel.h"

/*
  This is the function that each thread will execute on the GPU. The
  fact that it executes on the device is indicated by the __global__
  modifier in front of the return type of the function. After that,
  the signature of the function isn't special - in particular, the
  pointers we pass in should point to memory on the device, but this
  is not indicated by the function's signature.
 */
__global__ void reduce(int *in, int *out, int size) {

  // Create Shared Memory
  extern __shared__ int memPnt[];

  int tid = threadIdx.x;
  int index = blockIdx.x*blockDim.x*2 + threadIdx.x;
  int acc = 0;

  // First level reduction: Read from global into shared memory
  if( index < size){
    acc = in[index];
  }

  if(index + blockDim.x < size){
    acc += in[index+blockDim.x];
  }

  memPnt[tid] = acc;
  __syncthreads();

  // Warp Reduction
  for(int j=blockDim.x/2; j > 0; j>>=1){
    if(tid < j){
      // acc += memPnt[tid + j];
      memPnt[tid] = acc = acc + memPnt[tid + j];
    }

    __syncthreads();
  }
  if(tid == 0)
    out[blockIdx.x] = acc;
}
