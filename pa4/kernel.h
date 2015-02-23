#ifndef KERNEL_H_
#define KERNEL_H_

// This is the declaration of the function that will execute on the GPU.
__device__ bool isOdd(int);
__global__ void reduce(int *, int *, int);
__global__ void rReduce(int *, int *, int);

#endif // KERNEL_H_
