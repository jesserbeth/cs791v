#ifndef KERNEL_H_
#define KERNEL_H_

// This is the declaration of the function that will execute on the GPU.
__global__ void mandel(int *, int, float, float, float, float, int, int);

#endif // KERNEL_H_
