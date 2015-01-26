/*
  This program demonstrates the basics of working with cuda. We use
  the GPU to add two arrays. We also introduce cuda's approach to
  error handling and timing using cuda Events.

  This is the main program. You should also look at the header add.h
  for the important declarations, and then look at add.cu to see how
  to define functions that execute on the GPU.
 */

#include <iostream>
#include "stdio.h"
#include "add.h"

int main() {
  cudaDeviceProp prop; 	
  cudaError_t dev_err = cudaGetDeviceProperties(&prop, 0);
  if (dev_err != cudaSuccess) {
    std::cerr << "Error: " << cudaGetErrorString(dev_err) << std::endl;
    exit(1);
  }
  // printf("threads: %d \n", prop.maxThreadsPerBlock);
  // printf("blocks: %d \n", prop.maxThreadsDim[0]);
  // printf("grids: %d \n", prop.maxGridSize[0]);
  // printf("blocks: %d \n", prop.maxThreadsDim[1]);
  // printf("grids: %d \n", prop.maxGridSize[1]);
  // printf("blocks: %d \n", prop.maxThreadsDim[2]);
  // printf("grids: %d \n", prop.maxGridSize[2]);
  int max_threads = prop.maxThreadsPerBlock;
  int max_blocks = prop.maxThreadsDim[0];
  int max_input = 10000000;
  // Arrays on the host (CPU)
  // int a[N], b[N], c[N];
  int n, T, B;

  // bool flag = false;
  // Input size for N, B and T
  printf("Input Size of N (0 < N < 10,000,000): ");
	scanf(" %d", &n);
	// printf("N: %d \n", size);
	if(n <= 0)
		printf("Error: Wrong input value for N \n");
	else if( n > max_input)
		printf("Error: Size too large, setting N = 10,000,000 \n");
		n = 10000000;

	printf("Input Size of T (0 < T < 1024): ");
	scanf(" %d", &T);
	  
	if(T <= 0)
		printf("Error: Wrong input value for T \n");
	else if( T > max_threads)
		printf("Error: Size too large, setting T = 1024 \n");
		T = 1024;

	  printf("Input Size of B (0 < B < 1024): ");
	  scanf(" %d", &B);

	if(B <= 0){
	    printf("Error: Wrong input value for B \n");
	  }
	else if( B > max_blocks)
		printf("Error: Size too large");

  // Dynamically allocate arrays based on keyboard inputs
  int *a, *b, *c;
  a = (int*) malloc(n*sizeof(int));
  b = (int*) malloc(n*sizeof(int));
  c = (int*) malloc(n*sizeof(int));

  // a = (int*) malloc(n);
  // b = (int*) malloc(n);
  // c = (int*) malloc(n);

  /*
    These will point to memory on the GPU - notice the correspondence
    between these pointers and the arrays declared above.
   */
  int *dev_a, *dev_b, *dev_c;

  cudaError_t err = cudaMalloc( (void**) &dev_a, n * sizeof(int));
  if (err != cudaSuccess) {
    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
    exit(1);
  }
  err = cudaMalloc( (void**) &dev_b, n * sizeof(int));
  if (err != cudaSuccess) {
    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
    exit(1);
  }
  err = cudaMalloc( (void**) &dev_c, n * sizeof(int));
  if (err != cudaSuccess) {
    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
    exit(1);
  }

  // These lines just fill the host arrays with some data so we can do
  // something interesting. Well, so we can add two arrays.   */

  for (int i = 0; i < n; ++i) {
    a[i] = i;
    b[i] = i;
  }

  cudaEvent_t start, end;
  cudaEventCreate(&start);
  cudaEventCreate(&end);

  cudaEventRecord( start, 0 );

  cudaMemcpy(dev_a, a, n * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_b, b, n * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(dev_c, c, n * sizeof(int), cudaMemcpyHostToDevice);

  add<<<B,T>>>(dev_a, dev_b, dev_c, n);
  
  cudaMemcpy(c, dev_c, n * sizeof(int), cudaMemcpyDeviceToHost);

  cudaEventRecord( end, 0 );
  cudaEventSynchronize( end );

  float elapsedTime;
  cudaEventElapsedTime( &elapsedTime, start, end );

  /*
    Let's check that the results are what we expect.
   */
  for (int i = 0; i < n; ++i) {
    if (c[i] != a[i] + b[i]) {
      std::cerr << "Oh no! Something went wrong. You should check your cuda install and your GPU. :(" << std::endl;

      // clean up events - we should check for error codes here.
      cudaEventDestroy( start );
      cudaEventDestroy( end );

      // clean up device pointers - just like free in C. We don't have
      // to check error codes for this one.

      cudaFree(dev_a);
      cudaFree(dev_b);
      cudaFree(dev_c);
      free(a);
      free(b);
      free(c);
      exit(1);
    }
  }

  std::cout << "Yay! Your program's results are correct." << std::endl;
  std::cout << "Your program took: " << elapsedTime << " ms." << std::endl;
  
  // Cleanup in the event of success.
  cudaEventDestroy( start );
  cudaEventDestroy( end );



  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);

  free(a);
  free(b);
  free(c);
}
