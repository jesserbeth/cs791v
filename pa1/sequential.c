#include <iostream>
#include "stdio.h"
#include <sys/time.h>
#include <fstream> 

int main() {
  int max_input = 10000000;
  // Arrays on the host (CPU)
  // int a[N], b[N], c[N];
  int n;
  // std::ofstream out("seq.csv");

  // Input size for N, B and T
  printf("Input Size of N (0 < N < 10,000,000): ");
	scanf(" %d", &n);
	// printf("N: %d \n", size);
	if(n <= 0)
		printf("Error: Wrong input value for N \n");
	else if( n > max_input){
		printf("Error: Size too large, setting N = 10,000,000 \n");
		n = 10000000;
  	}
  	
	// for(int j = 1000000; j < 100000001; j += 500000){
  	  int j = n;
  	  struct timeval start, fin;
	  // Dynamically allocate arrays based on keyboard inputs
	  int *a, *b, *c;
	  a = new int [j*sizeof(int)];
	  b = new int [j*sizeof(int)];
	  c = new int [j*sizeof(int)];

	  // These lines just fill the host arrays with some data so we can do
	  // something interesting. Well, so we can add two arrays.   */

	  for (int i = 0; i < j; ++i) {
	    a[i] = i;
	    b[i] = i;
	  }

	  gettimeofday(&start, NULL);
	  for(int i = 0; i < j; i++){
	  	c[i] = a[i] + b[i];
	  }
	  gettimeofday(&fin, NULL);

	  double t = fin.tv_usec + fin.tv_sec * 1000000.0;
	  t -= start.tv_usec + start.tv_sec * 1000000.0;
	  t /= 1000000.0;
	  std::cout << "Processing " << j << " inputs took " << t << " microseconds" << std::endl;
	  // out << t << std::endl;
	  /*
	    Let's check that the results are what we expect.
	   */
	  for (int i = 0; i < j; ++i) {
	    if (c[i] != a[i] + b[i]) {
	      std::cerr << "Oh no! Something went wrong. You should check your cuda install and your GPU. :(" << std::endl;

	    }
	  }
		delete a;
		delete b;
		delete c;
	// }
}	
