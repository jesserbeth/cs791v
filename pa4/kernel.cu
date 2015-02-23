
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
    // First level reduction: Read from global into shared memory
    // if(index + 1 < size){
    //   acc += in[index+1];
    // }
	   // if(index + blockDim.x < size){
	   //   acc += in[index+blockDim.x];
	   // }
    // if(index + blockDim.x < size){
    //   acc += in[index+blockDim.x];
    // }
    
  //  index += blockDim.x * gridDim.x *2;
    index += blockDim.x * gridDim.x;
 }

    memPnt[tid] = acc;
    __syncthreads();

  // for(int j=blockDim.x/2; j > 0; j>>=1){
  //   if(tid < j){
  //     memPnt[tid] = acc = acc + memPnt[tid + j];
  //   }

  // // for(int j=blockDim.x/2; j > 0; j/=2){
  // //   if(tid < j){
  // //     memPnt[tid] = acc = acc + memPnt[tid + j];
  // //   }
  // //   __syncthreads();
  // }




	// do reduction in shared mem
	// unsigned int r = size/2;
	// bool odd = false;
    // int r = size;
	for(int r = blockDim.x; r > 0; r/=2){
		if(blockDim.x >= r && tid < r/2){
	    	memPnt[tid] = acc = acc + memPnt[tid + r/2];
	    	// memPnt[tid] += memPnt[tid + r/2];
		    if(isOdd(r) && tid == 0)
		    	memPnt[tid] += memPnt[r-1];
		}
		
		__syncthreads();
	}



    // if ((blockDim.x >=   2) && ( tid <  1))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid +  1];
    // }

    // __syncthreads();

	// if (/*(size >= 512) &&*/ (tid < r))
	// // if ((blockDim.x >= 1024) && (tid < 512))
	// {
	//     // memPnt[tid] = acc = acc + memPnt[tid + 512];
	//     memPnt[tid] = acc = acc + memPnt[tid + r*2];
	//     if(isOdd(r) && tid ==0)
	//     	memPnt[tid] += memPnt[r*2-1];
	// }

	// __syncthreads();
	// r /= 2;
	// if (/*(size >= 512) &&*/ (tid < r))
	// // if ((blockDim.x >= 512) && (tid < 256))
	// {
	//     // memPnt[tid] = acc = acc + memPnt[tid + 256];
	//     memPnt[tid] = acc = acc + memPnt[tid + r*2];
	//     if(isOdd(r) && tid ==0)
	//     	memPnt[tid] += memPnt[r*2-1];
	// }

	// __syncthreads();

	// r /= 2;
	// if (/*(size >= 256) &&*/ (tid < r))
	// // if ((blockDim.x >= 256) && (tid < 128))
	// {
	//         // memPnt[tid] = acc = acc + memPnt[tid + 128];
	//         memPnt[tid] = acc = acc + memPnt[tid + r*2];
	//     if(isOdd(r) && tid ==0)
	//     	memPnt[tid] += memPnt[r*2-1];
	// }

	//  __syncthreads();

	// r /= 2;
	// if (/*(size >= 128) &&*/ (tid <  r))
	// // if ((blockDim.x >= 128) && (tid <  64))
	// {
	//    // memPnt[tid] = acc = acc + memPnt[tid +  64];
	//    memPnt[tid] = acc = acc + memPnt[tid +  r*2];
	//     if(isOdd(r) && tid ==0)
	//     	memPnt[tid] += memPnt[r*2-1];
	// }

	// __syncthreads();




    // // fully unroll reduction within a single warp
    // if ((blockDim.x >=  64) && (tid < 32))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid + 32];
    // }

    // __syncthreads();

    // if ((blockDim.x >=  32) && (tid < 16))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid + 16];
    // }

    // __syncthreads();

    // if ((blockDim.x >=  16) && (tid <  8))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid +  8];
    // }

    // __syncthreads();

    // if ((blockDim.x >=   8) && (tid <  4))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid +  4];
    // }

    // __syncthreads();

    // if ((blockDim.x >=   4) && (tid <  2))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid +  2];
    // }

    // __syncthreads();

    // if ((blockDim.x >=   2) && ( tid <  1))
    // {
    //     memPnt[tid] = acc = acc + memPnt[tid +  1];
    // }

    // __syncthreads();

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
      printf("%d, %d\n", tid, size);
      rReduce<<<gridDim.x, blockDim.x, memorySize*sizeof(int)>>>(in, out, size);
  }

  __threadfence();

}