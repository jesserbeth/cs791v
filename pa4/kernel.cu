
#include "kernel.h"
#include <stdio.h>

__device__ bool isOdd(int val){
	if(val % 2 == 0){
		return false;
	}
	return true;
}
__device__ int loc_ceil(float val){
  int tmp = (int)val;
  if(tmp == val)
    return val;
  else
    return tmp + 1;
}
__global__ void reduce(int *in, int *out, int size) {

  // Create Shared Memory
  extern __shared__ int memPnt[];

  int tid = threadIdx.x;
  // int index = blockIdx.x*blockDim.x*2 + threadIdx.x;
  int index = blockIdx.x*blockDim.x + threadIdx.x;
  int acc = 0;

  // Here is where striding is accomplished
  while(index < size){
    if( index < size){
      acc += in[index];
    }
    index += blockDim.x * gridDim.x;
 }

    memPnt[tid] = acc;
    __syncthreads();

	for(int r = blockDim.x; r > 0; r/=2){
		if(blockDim.x >= r && tid < r/2){
	    	memPnt[tid] = acc = acc + memPnt[tid + r/2];
	    	// memPnt[tid] += memPnt[tid + r/2];
		    if(isOdd(r) && tid == 0)
		    	memPnt[tid] += memPnt[r-1];
		}
		
		__syncthreads();
	}


  if(tid == 0){
    // out[blockIdx.x] = acc;
    out[blockIdx.x] = memPnt[tid];
  }

}

__global__ void rReduce(int *in, int *out, int size) {

  extern __shared__ int memPnt[];

  int tid = threadIdx.x;
  // int index = blockIdx.x*blockDim.x*2 + threadIdx.x;
  int index = blockIdx.x*blockDim.x + threadIdx.x;
  int acc = 0;

  // Here is where striding is accomplished
  while(index < size){
    if( index < size){
      acc += in[index];
    }
    index += blockDim.x * gridDim.x;
 }

    memPnt[tid] = acc;
    __syncthreads();


  for(int r = blockDim.x; r > 0; r/=2){
    if(blockDim.x >= r && tid < r/2){
        memPnt[tid] = acc = acc + memPnt[tid + r/2];
        if(isOdd(r) && tid == 0)
          memPnt[tid] += memPnt[r-1];
    }
    
    __syncthreads();
  }


  if(tid == 0){
    out[blockIdx.x] = memPnt[tid];
  }

  // Recalculate size
  size = loc_ceil(size / (blockDim.x*2));
  if(size > gridDim.x)
    size = gridDim.x;

  // synchronize across blocks
  __threadfence();

    int memorySize = gridDim.x*2;
    if(gridDim.x <= 32)
      memorySize = 64;

    int *temp = in;
    in = out;
    out = temp;

  // Call next kernel
  if(blockIdx.x*blockDim.x + threadIdx.x == 0 && size >= 1){
      // printf("%d, %d\n", tid, size);
      rReduce<<<gridDim.x, blockDim.x, memorySize*sizeof(int)>>>(in, out, size);
  }

  __threadfence();

}