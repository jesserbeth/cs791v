/*
  PROGRAMMING ASSIGNMENT 1
  JESSICA SMITH
  CS791V
 */

#include <iostream>
#include "stdio.h"
#include "kernel.h"
#include <fstream> 

int main() {
  int n, T, B;
  // n = pow(2,21);
  // n = 2097087;
  int check = 0;
  int result = 0;
  float calcTime, memTransTime;
  char version;
  // std::ofstream out("ParVSSeq.csv");
  // std::ofstream out("test.csv");

  // Handle I/O from user
  printf("Input program version (a, b, c):\na: Multiple cpu kernel calls\nb: Kernel calls kernel\nc: CPU finishes the sum\n");
  scanf(" %c", &version);
  if(version != 'a' && version != 'b' && version != 'c'){
  	printf("Error: Invalid input.\n");
  	return 0;
  }

  // Input size for N, B and T
  printf("Input Size of N (0 < N < 100,000,000): ");
	scanf(" %d", &n);
	// printf("N: %d \n", size);
	if(n <= 0)
		printf("Error: Wrong input value for N \n");
	else if( n > 100000000){
		printf("Error: Size too large, setting N = 100,000,000 \n");
		n = 100000000;
	}

	printf("Input Size of T (0 < T < 1024): ");
	scanf(" %d", &T);
	  
	if(T <= 0)
		printf("Error: Wrong input value for T \n");
	else if( T > 1024){
		printf("Error: Size too large, setting T = 1024 \n");
		T = 1024;
	}

	printf("Input Size of B (0 < B < 1024): ");
	scanf(" %d", &B);

	if(B <= 0){
	    printf("Error: Wrong input value for B \n");
	  }
	else if( B > 56000){
		printf("Error: Size too large");
		B = 56000;
	}

	  int memorySize = B*2;
	  if(B <= 32)
	  	memorySize = 64;
	  int *input, *output;
	  input = (int*) malloc(n*sizeof(int));
	  output = (int*) malloc(B*sizeof(int));

	  int *g_in, *g_out;

	  cudaError_t err = cudaMalloc( (void**) &g_in, n * sizeof(int));
	  err = cudaMalloc( (void**) &g_out, B * sizeof(int));
	  if (err != cudaSuccess) {
	    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
	    exit(1);
	  }

	  // Populate Array for addition
	  for (int i = 0; i < n; ++i) {
	    input[i] = 1;
	    // input[i] = i%3;
	  }
	  // Initialize output array to avoid garbage additions
	  for(int i = 0; i < B; i++){
	  	output[i] = 0;
	  }

	  // Create cuda Events
	  cudaEvent_t start, end, m_start, m_end;
	  cudaEventCreate(&start);
	  cudaEventCreate(&end);
	  cudaEventCreate(&m_start);
	  cudaEventCreate(&m_end);

	  cudaEventRecord( m_start, 0 );

	  err = cudaMemcpy(g_out, output, B * sizeof(int), cudaMemcpyHostToDevice);
	  err = cudaMemcpy(g_in, input, n * sizeof(int), cudaMemcpyHostToDevice);
	  if (err != cudaSuccess) {
	    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
	    exit(1);
	  }  

	  // Different calls for each version of code
	  if(version == 'a'){
	  	// n = B * T;
		  int s = n;
		  // int s = B;
		  // int count = 0;
		  cudaEventRecord(start,0);
		  // std::cout << memorySize << std::endl;
		  while( s > 1){
		    std::cout << "S IS: " << s << std::endl;
		  	// Call Kernel:
		  	reduce<<<B,T,B*T*sizeof(int)>>>(g_in, g_out, s);
		  	
		  	// Adjust new processing size:
		  	s = ceil(s / (T*2));
		  	if(s > B)
		  		s = B;
		  	// s = (s + (T*2-1)) / (T*2);

		  	// Swap pointers to loop
		  	int *temp = g_in;
		  	g_in = g_out;
		  	g_out = temp;
		  	// Count the number of iterations to determine which variable holds the final sum
		  	// std::cout << count << std::endl;
		  	// s = 0;
		  	// count++;

	    	err = cudaMemcpy(output, g_in, B * sizeof(int), cudaMemcpyDeviceToHost);
	    	if (err != cudaSuccess) {
	    	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
	    	  exit(1);
		  	}
		  		std::cout << "OUTPUT VECTOR: " << std::endl;
		  	for(int i = 0; i < B; i++){
		  		std::cout << output[i] << std::endl;
		  	}
		  }
		  cudaEventRecord(end, 0);
		  cudaEventSynchronize(end);

	    	err = cudaMemcpy(output, g_in, B * sizeof(int), cudaMemcpyDeviceToHost);
	    	if (err != cudaSuccess) {
	    	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
	    	  exit(1);
		  	}

		  cudaEventElapsedTime( &calcTime, start, end );
		  
		  cudaEventRecord( m_end, 0 );
		  cudaEventSynchronize( m_end );
		  
		  cudaEventElapsedTime( &memTransTime, m_start, m_end );

		  // Store result to variable
		  result = output[0] + output[1];
		  // result = output[0];
		  for(int i = 0; i < B; i++)
		  	std::cout << "output: " << output[i] << '\n';
	  }
	  else if(version == 'c'){
		  cudaEventRecord(start,0);	
		  reduce<<<B,T,memorySize*sizeof(int)>>>(g_in, g_out, n);
		  cudaEventRecord(end, 0);
		  cudaEventSynchronize(end);

		  err = cudaMemcpy(output, g_out, B * sizeof(int), cudaMemcpyDeviceToHost);
	    	if (err != cudaSuccess) {
	    	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
	    	  exit(1);
		  	}
		  	// std::cout << "output[0]" << output[0] << std::endl;

		  result = 0;
		  for(int i = 0; i < B; i++){
		  	result += output[i];
		  	// std::cout << output[i] << std::endl;
		  }
		  cudaEventElapsedTime( &calcTime, start, end );
		  
		  cudaEventRecord( m_end, 0 );
		  cudaEventSynchronize( m_end );
		  
		  cudaEventElapsedTime( &memTransTime, m_start, m_end );

		  // Correctness check
		  result = 0;
		  for(int i = 0; i < B; i++){
		  	result += output[i];
		  	std::cout << output[i] << std::endl;
		  }
	  }
	  else if(version == 'b'){
	  	// Recursive Kernel
		  cudaEventRecord(start,0);	
		  rReduce<<<B,T,memorySize*sizeof(int)>>>(g_in, g_out, n);
		  cudaEventRecord(end, 0);
		  cudaEventSynchronize(end);


		  err = cudaMemcpy(output, g_in, B * sizeof(int), cudaMemcpyDeviceToHost);
	    	if (err != cudaSuccess) {
	    	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
	    	  exit(1);
		  	}
		  	// std::cout << "output[0]" << output[0] << std::endl;

		  result = 0;
		  for(int i = 0; i < B; i++){
		  	result += output[i]; 
		  	// std::cout << output[i] << std::endl;
		  }
		  cudaEventElapsedTime( &calcTime, start, end );
		  
		  cudaEventRecord( m_end, 0 );
		  cudaEventSynchronize( m_end );
		  
		  cudaEventElapsedTime( &memTransTime, m_start, m_end );

		  // Correctness check
		  // Store result to variable
		  result = output[0] + output[1];
		  // result = output[0];
		  for(int i = 0; i < B; i++)
		  	std::cout << "output: " << output[i] << '\n';
	  }

		  // result = 0;
		  // for(int i = 0; i < B; i++){
		  // 	result += output[i];
		  // 	// std::cout << output[i] << std::endl;
		  // }
	  check = 0;
	  for(int i = 0; i < n; i++){
	  	check += input[i];
	  }

	  if(check != result){
	  	std::cerr << "Oh no! Something went wrong. You Suck. :(" << std::endl;
	  	std::cerr << result << " " << check << std::endl;
	  	// std::cerr << result - check << std::endl;
	  	cudaFree(g_out);
	  	cudaFree(g_in);
	  	free(input);
	  	free(output);
	  	exit(1);
	  }
	  
	  // std::cout << "Yay! Your program's results are correct." << std::endl;
	  std::cout << std::endl;
	  std::cout << "Your program took: " << memTransTime/1000 << " seconds With Memory Transfer on " << n << " inputs" << std::endl;
	  std::cout << "Your program took: " << calcTime/1000 << " seconds Without Memory Transfer on " <<  n << " inputs" << std::endl;
	  
	  // Cleanup in the event of success.
	  cudaEventDestroy( start );
	  cudaEventDestroy( end );
	  cudaEventDestroy( m_start );
	  cudaEventDestroy( m_end );

	  // // write to file
	  // int threads = i;
	  // int blocks = j;
	  // double memThrough = n / memTransTime;
	  // double calcThrough = n / calcTime;
	  // out << memThrough << ',' << calcThrough << ',' << T << ',' << B << '\n' ;
	  // out << memThrough << ',' << calcThrough << ',' << n << '\n' ;
	  // std::cout << memThrough << ',' << calcThrough << ',' << T << ',' << B << '\n' ;
	  // std::cout << std::endl;
	  // std::cout << "Throughput: " << memThrough << " ms. With Memory Transfer on " <<  n << " inputs" << std::endl;
	  // std::cout << "Throughput: " << calcThrough << " ms. Without Memory Transfer on " <<  n << " inputs" << std::endl;
	  
	  cudaFree(g_in);
	  cudaFree(g_out);

	  free(input);
	  free(output);

  // out.close();
  // }
}	
