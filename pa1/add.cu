
#include "add.h"

/*
  This is the function that each thread will execute on the GPU. The
  fact that it executes on the device is indicated by the __global__
  modifier in front of the return type of the function. After that,
  the signature of the function isn't special - in particular, the
  pointers we pass in should point to memory on the device, but this
  is not indicated by the function's signature.
 */
__global__ void add(int *a, int *b, int *c, int size, bool striding) {
  if(striding == true){
	int thread_id = blockIdx.x * blockDim.x + threadIdx.x;
	while( thread_id < size ){
	// if(thread_id < size)  
	  c[thread_id] = a[thread_id] + b[thread_id];
	  thread_id += blockDim.x * gridDim.x;  	
  	}
  }
  else{
  	int tid = blockIdx.x * blockDim.x + threadIdx.x;
  	if(tid<size){
  		c[tid] = a[tid] + b[tid];
  	}
  }
}
