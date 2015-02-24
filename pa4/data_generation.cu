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
  // int n, T, B;
  // n = pow(2,21);
  // n = 2097087;
  int check = 0;
  int result = 0;
  float calcTime, memTransTime;
  // char version;
  // std::ofstream out("ParVSSeq.csv");
  // std::ofstream out("test.csv");


// Loop to generate data points (blocks/threads/inputsize)
  for(int n = 1000000; n <= 110000000; n += 20000000){
		char numstr[21];
		sprintf(numstr, "%d", n);
		char f_name[] = "Recursive_Call_";
		char ppm[] = ".csv";
		strcat(f_name,numstr);
		strcat(f_name,ppm);
	  std::ofstream out(f_name);
	  for(int B = 200; B < 56000; B += 4096){
	  	for(int T = 32; T <= 1024; T<<=1){
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
			  // if(version == 'a'){
				 //  int s = n;
				 //  cudaEventRecord(start,0);
				 //  // std::cout << memorySize << std::endl;
				 //  while( s > 1){
				 //    // std::cout << "S IS: " << s << std::endl;
				 //  	// Call Kernel:
				 //  	reduce<<<B,T,B*T*sizeof(int)>>>(g_in, g_out, s);
				  	
				 //  	// Adjust new processing size:
				 //  	s = ceil(s / (T*2));
				 //  	if(s > B)
				 //  		s = B;

				 //  	// Swap pointers to loop
				 //  	int *temp = g_in;
				 //  	g_in = g_out;
				 //  	g_out = temp;

			  //   // 	err = cudaMemcpy(output, g_in, B * sizeof(int), cudaMemcpyDeviceToHost);
			  //   // 	if (err != cudaSuccess) {
			  //   // 	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
			  //   // 	  exit(1);
				 //  	// }
				 //  	// 	std::cout << "OUTPUT VECTOR: " << std::endl;
				 //  	// for(int i = 0; i < B; i++){
				 //  	// 	std::cout << output[i] << std::endl;
				 //  	// }
				 //  }
				 //  cudaEventRecord(end, 0);
				 //  cudaEventSynchronize(end);

			  //   	err = cudaMemcpy(output, g_in, 1 * sizeof(int), cudaMemcpyDeviceToHost);
			  //   	if (err != cudaSuccess) {
			  //   	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
			  //   	  exit(1);
				 //  	}

				 //  cudaEventElapsedTime( &calcTime, start, end );
				  
				 //  cudaEventRecord( m_end, 0 );
				 //  cudaEventSynchronize( m_end );
				  
				 //  cudaEventElapsedTime( &memTransTime, m_start, m_end );

				 //  // Store result to variable
				 //  // result = output[0] + output[1];
				 //  // result = output[0];
				 //  // for(int i = 0; i < B; i++)
				 //  // 	std::cout << "output: " << output[i] << '\n';
			  // cudaEventDestroy( start );
			  // cudaEventDestroy( end );
			  // cudaEventDestroy( m_start );
			  // cudaEventDestroy( m_end );

			  // // // write to file
			  // int threads = T;
			  // int blocks = B;
			  // // double memThrough = (n*sizeof(int)) / (memTransTime/1000);
			  // double calcThrough = (n*sizeof(int)*1.0*pow(10,-9)) / (calcTime/1000);
			  // out << calcThrough << ',' << "Gb/s" << ',' << T << ','  << "Threads" << ',' << B << ',' << "Blocks" << ',' << n << '\n' ;
			  // // out << memThrough << ',' << calcThrough << ',' << n << '\n' ;
			  // // std::cout << memThrough << ',' << calcThrough << ',' << T << ',' << B << '\n' ;
			  // // std::cout << std::endl;
			  // // std::cout << "Throughput: " << memThrough << " ms. With Memory Transfer on " <<  n << " inputs" << std::endl;
			  // // std::cout << "Throughput: " << calcThrough << " ms. Without Memory Transfer on " <<  n << " inputs" << std::endl;
			  
			  // cudaFree(g_in);
			  // cudaFree(g_out);

			  // free(input);
			  // free(output);
			  // }
			  // else if(version == 'c'){
				  // cudaEventRecord(start,0);	
				  // reduce<<<B,T,B*T*sizeof(int)>>>(g_in, g_out, n);
				  // cudaEventRecord(end, 0);
				  // cudaEventSynchronize(end);

				  // err = cudaMemcpy(output, g_out, 1 * sizeof(int), cudaMemcpyDeviceToHost);
			   //  	if (err != cudaSuccess) {
			   //  	  std::cerr << "Out Copy Error: " << cudaGetErrorString(err) << std::endl;
			   //  	  exit(1);
				  // 	}
				  // 	// std::cout << "output[0]" << output[0] << std::endl;

				  // result = 0;
				  // for(int i = 0; i < B; i++){
				  // 	result += output[i];
				  // 	// std::cout << output[i] << std::endl;
				  // }
				  // cudaEventElapsedTime( &calcTime, start, end );
				  
				  // cudaEventRecord( m_end, 0 );
				  // cudaEventSynchronize( m_end );
				  
				  // cudaEventElapsedTime( &memTransTime, m_start, m_end );

				  // // Correctness check
				  // // result = 0;
				  // // for(int i = 0; i < B; i++){
				  // // 	result += output[i];
				  // // 	// std::cout << output[i] << std::endl;
				  // // }

				  // cudaEventDestroy( start );
				  // cudaEventDestroy( end );
				  // cudaEventDestroy( m_start );
				  // cudaEventDestroy( m_end );

				  // // // write to file
				  // int threads = T;
				  // int blocks = B;
				  // // double memThrough = (n*sizeof(int)) / (memTransTime/1000);
				  // double calcThrough = (n*sizeof(int)*1.0*pow(10,-9)) / (calcTime/1000);
				  // out << calcThrough << ',' << "Gb/s" << ',' << T << ','  << "Threads" << ',' << B << ',' << "Blocks" << ',' << n << '\n' ;
				  // // out << memThrough << ',' << calcThrough << ',' << n << '\n' ;
				  // // std::cout << memThrough << ',' << calcThrough << ',' << T << ',' << B << '\n' ;
				  // // std::cout << std::endl;
				  // // std::cout << "Throughput: " << memThrough << " ms. With Memory Transfer on " <<  n << " inputs" << std::endl;
				  // // std::cout << "Throughput: " << calcThrough << " ms. Without Memory Transfer on " <<  n << " inputs" << std::endl;
				  
				  // cudaFree(g_in);
				  // cudaFree(g_out);

				  // free(input);
				  // free(output);
			  // }
			  // else if(version == 'b'){
			  	// Recursive Kernel
				  cudaEventRecord(start,0);	
				  rReduce<<<B,T,B*T*sizeof(int)>>>(g_in, g_out, n);
				  cudaEventRecord(end, 0);
				  cudaEventSynchronize(end);


				  err = cudaMemcpy(output, g_in, 1 * sizeof(int), cudaMemcpyDeviceToHost);
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
				  // result = output[0] + output[1];
				  // result = output[0];
				  // for(int i = 0; i < B; i++)
				  // 	std::cout << "output: " << output[i] << '\n';

				  cudaEventDestroy( start );
				  cudaEventDestroy( end );
				  cudaEventDestroy( m_start );
				  cudaEventDestroy( m_end );

				  // // write to file
				  int threads = T;
				  int blocks = B;
				  // double memThrough = (n*sizeof(int)) / (memTransTime/1000);
				  double calcThrough = (n*sizeof(int)*1.0*pow(10,-9)) / (calcTime/1000);
				  out << calcThrough << ',' << "Gb/s" << ',' << T << ','  << "Threads" << ',' << B << ',' << "Blocks" << ',' << n << '\n' ;
				  // out << memThrough << ',' << calcThrough << ',' << n << '\n' ;
				  // std::cout << memThrough << ',' << calcThrough << ',' << T << ',' << B << '\n' ;
				  // std::cout << std::endl;
				  // std::cout << "Throughput: " << memThrough << " ms. With Memory Transfer on " <<  n << " inputs" << std::endl;
				  // std::cout << "Throughput: " << calcThrough << " ms. Without Memory Transfer on " <<  n << " inputs" << std::endl;
				  
				  cudaFree(g_in);
				  cudaFree(g_out);

				  free(input);
				  free(output);
			  // }

			  // check = 0;
			  // for(int i = 0; i < n; i++){
			  // 	check += input[i];
			  // }

			  // if(check != result){
			  // 	std::cerr << "Oh no! Something went wrong. You Suck. :(" << std::endl;
			  // 	std::cerr << result << " " << check << std::endl;
			  // 	// std::cerr << result - check << std::endl;
			  // 	cudaFree(g_out);
			  // 	cudaFree(g_in);
			  // 	free(input);
			  // 	free(output);
			  // 	exit(1);
			  // }
			  
			  // std::cout << "Yay! Your program's results are correct." << std::endl;
			  // std::cout << std::endl;
			  // std::cout << "Your program took: " << memTransTime/1000 << " seconds With Memory Transfer on " << n << " inputs" << std::endl;
			  // std::cout << "Your program took: " << calcTime/1000 << " seconds Without Memory Transfer on " <<  n << " inputs" << std::endl;
			  
			  // Cleanup in the event of success.

		  // out.close();
		  // }


	  	}
	  	// printf("\n");
	  }
	  	printf("%d,\n",n);
	  	out.close();
  }
}	