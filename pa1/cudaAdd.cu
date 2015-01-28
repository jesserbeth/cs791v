/*
  PROGRAMMING ASSIGNMENT 1
  JESSICA SMITH
  CS791V
 */

#include <iostream>
#include "stdio.h"
#include "add.h"
#include <fstream> 

int main() {
  cudaDeviceProp prop; 	
  cudaError_t dev_err = cudaGetDeviceProperties(&prop, 0);
  if (dev_err != cudaSuccess) {
    std::cerr << "Error: " << cudaGetErrorString(dev_err) << std::endl;
    exit(1);
  }
  int max_threads = prop.maxThreadsPerBlock;
  int max_blocks = prop.maxThreadsDim[0];
  int max_input = 10000000;
  int max_seq;
  bool striding = true;
  int n, T, B;

  printf("Use striding? (No: 0, Yes: 1): ");
  scanf(" %d", &n);
  if(n == 0)
  	striding = false;
  else if(n == 1)
  	striding = true;
  else{
  	printf("Error, Invalid input. Aborting.\n");
  	return 0;
  }

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

	max_seq = B * T;
	// Check for striding need
	if(n > max_seq && striding == false){
		printf("Striding is needed for this input size: Using Striding.\n");
		striding = true;
	}
  
  // std::ofstream out("no_striding_max_input.csv");

  // n = 1048575;
  // Dynamically allocate arrays based on keyboard inputs
  int *a, *b, *c;
  a = (int*) malloc(n*sizeof(int));
  b = (int*) malloc(n*sizeof(int));
  c = (int*) malloc(n*sizeof(int));

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

  // for(int i = 10; i < 1024; i += 100){
  // 	for(int j = 10; j < 1024; j += 100){
  		  // T = i;
  		  // B = j;
  		  // n = B * T;

		  cudaEvent_t start, end;
		  cudaEventCreate(&start);
		  cudaEventCreate(&end);

		  cudaEventRecord( start, 0 );

		  err = cudaMemcpy(dev_a, a, n * sizeof(int), cudaMemcpyHostToDevice);
		  err = cudaMemcpy(dev_b, b, n * sizeof(int), cudaMemcpyHostToDevice);
		  err = cudaMemcpy(dev_c, c, n * sizeof(int), cudaMemcpyHostToDevice);
		 
		  if (err != cudaSuccess) {
		    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
		    exit(1);
		  }

		  add<<<B,T>>>(dev_a, dev_b, dev_c, n, striding);

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

		  // // write to file
		  // int threads = i;
		  // int blocks = j;
		  // out << elapsedTime << ',' << threads << ',' << blocks << '\n' ;

  // 	} 
  // 	std::cout << 't' << std::endl;
  // }
		  cudaFree(dev_a);
		  cudaFree(dev_b);
		  cudaFree(dev_c);

		  free(a);
		  free(b);
		  free(c);

  // out.close();
}	
