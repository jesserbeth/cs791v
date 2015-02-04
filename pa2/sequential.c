#include <iostream>
#include "stdio.h"
#include <sys/time.h>
#include <fstream> 
#include <math.h>

int main() {
  int n;
  std::ofstream out("seq.csv");

  for(int p = 18; p < 24; p++){

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
