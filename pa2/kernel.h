/*
  This header demonstrates how we build cuda programs spanning
  multiple files. 
 */

#ifndef KERNEL_H_
#define KERNEL_H_

// This is the declaration of the function that will execute on the GPU.
__global__ void reduce(int *, int *, int);

#endif // KERNEL_H_
