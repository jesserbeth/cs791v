/*
  PROGRAMMING ASSIGNMENT 3
  JESSICA SMITH
  CS791V
 */

#include <iostream>
#include "stdio.h"
#include "kernel.h"
#include <fstream> 

int main() {
	int H = 2000;
	int W = 2000;
	int P = H * W;
	int B = 0;
	int iterations = 1024;
	float calcTime, memTime;

	// Mandelbrot Variables
	double MinRe = -2.0;
	double MaxRe = 1.0;
	double MinIm = -1.2;
	double MaxIm = MinIm + (MaxRe-MinRe)*H/W;
	std::ofstream data_out("parallel_data.csv");

	// For loop to generate plot data
	for(int T = 64; T < 1025; T<<=1){
		B = ceil(P / T);

		char numstr[21];
		char threadNum[21];
		sprintf(numstr, "%d", iterations);
		sprintf(threadNum, "_%d", T);
		char f_name[] = "man_par_";
		char ppm[] = ".ppm";
		strcat(f_name,numstr);
		strcat(f_name,threadNum);
		strcat(f_name,ppm);

		std::ofstream im_out(f_name);

		// Declare img vectors
		int *img;
		img = (int*)malloc(P *sizeof(int));
		// Initialize Image
		for(int i = 0; i < P; i++){
			img[i] = 255;
		}

		int *g_out;

		cudaError_t err = cudaMalloc( (void**) &g_out, P*sizeof(int));
		if (err != cudaSuccess) {
		    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
		    exit(1);
	    }

	    // Create cuda events
	    cudaEvent_t start, end, m_start, m_end;
	    cudaEventCreate(&start);
	    cudaEventCreate(&end);
	    cudaEventCreate(&m_start);
	    cudaEventCreate(&m_end);

	    // Start memory event recording
	    cudaEventRecord(m_start, 0);

	    err = cudaMemcpy(g_out, img, P * sizeof(int), cudaMemcpyHostToDevice);
		if (err != cudaSuccess) {
		    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
		    exit(1);
	    }

	    // Start the kernel
	    cudaEventRecord(start, 0);
	    mandel<<<B,T>>>(g_out, iterations, MinRe, MaxRe, MinIm, MaxIm, W, H);
	    cudaEventRecord(end, 0);
	    cudaEventSynchronize(end);

	    cudaEventElapsedTime( &calcTime, start, end);

	    err = cudaMemcpy(img, g_out, P * sizeof(int), cudaMemcpyDeviceToHost);
		if (err != cudaSuccess) {
		    std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
		    exit(1);
	    }

	    cudaEventRecord(m_end, 0);
	    cudaEventSynchronize(m_end);

	    cudaEventElapsedTime(&memTime, m_start, m_end);

		std::cout << std::endl;
		std::cout << "Your program took: " << memTime << " ms. With Memory Transfer with " << T << " threads and " << B << " blocks" << std::endl;
		std::cout << "Your program took: " << calcTime << " ms. Without Memory Transfer with" <<  T  << " threads and " << B << " blocks" << std::endl;

		cudaEventDestroy( start );
		cudaEventDestroy( end );
		cudaEventDestroy( m_start );
		cudaEventDestroy( m_end );

		// Write to file
		data_out << memTime << ',' << calcTime << ',' << T << ',' << B << '\n' ;

		// Write to image:
		im_out << "P2" << '\n';
		im_out << "2000 2000" << '\n';
		im_out << "255" << '\n';

		for(int y = 0; y < W; y++){
			for(int x = 0; x < H; x++){
				im_out << img[y*W + x] << " ";
				// std::cout << img[y*W + x] << " ";
				if(img[y*W + x] == 0)
					std::cout << "Zero\n";
			}
			im_out << "\n";
			// std::cout << "\n";
		}
	  
		cudaFree(g_out);

		free(img);
		im_out.close();	
	}
	data_out.close();
}	
