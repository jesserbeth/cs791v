#include <iostream>
#include "stdio.h"
#include <sys/time.h>
#include <fstream> 
#include <math.h>

int main() {
  int n;
  std::ofstream out("seq.csv");

  for(int p = 18; p < 24; p++){
  // 	  int j = n;
  // 	  struct timeval start, fin;
	 //  // Dynamically allocate arrays based on keyboard inputs
	 //  int *a, *b, *c;
	 //  a = new int [j*sizeof(int)];
	 //  b = new int [j*sizeof(int)];
	 //  c = new int [j*sizeof(int)];

	 //  // These lines just fill the host arrays with some data so we can do
	 //  // something interesting. Well, so we can add two arrays.   */

	 //  for (int i = 0; i < j; ++i) {
	 //    a[i] = i;
	 //    b[i] = i;
	 //  }

	 //  gettimeofday(&start, NULL);
	 //  for(int i = 0; i < j; i++){
	 //  	c[i] = a[i] + b[i];
	 //  }
	 //  gettimeofday(&fin, NULL);

	 //  double t = fin.tv_usec + fin.tv_sec * 1000000.0;
	 //  t -= start.tv_usec + start.tv_sec * 1000000.0;
	 //  t /= 1000000.0;
	 //  std::cout << "Processing " << j << " inputs took " << t << " microseconds" << std::endl;
	 //  // out << t << std::endl;
	 //  /*
	 //    Let's check that the results are what we expect.
	 //   */
	 //  for (int i = 0; i < j; ++i) {
	 //    if (c[i] != a[i] + b[i]) {
	 //      std::cerr << "Oh no! Something went wrong. You should check your cuda install and your GPU. :(" << std::endl;

	 //    }
	 //  }
		// delete a;
		// delete b;
		// delete c;

  	  n = pow(2,p);
  	  struct timeval start, fin;
  	  int *a;
  	  int count = 0;
	  a = new int [n*sizeof(int)];

	  for(int i = 0; i < n; i++){
	  	a[i] = 1;
	  }

	  gettimeofday(&start, NULL);
	  for(int i = 0; i < n; i++){
	  	count += a[i];
	  }
	  gettimeofday(&fin, NULL);

	  double t = fin.tv_usec + fin.tv_sec * 1000000.0;
	  t -= start.tv_usec + start.tv_sec * 1000000.0;
	  t /= 1000000.0;
	  std::cout << "Processing " << n << " inputs took " << t << " microseconds" << std::endl;
	  t /= .001;
	  out << t << ',' << n << std::endl;
	  /*
	    Let's check that the results are what we expect.
	   */
	    if (count != n) {
	      std::cerr << "Oh no! Something went wrong. You should check your cuda install and your GPU. :(" << std::endl;
	  	}
	}
}	
