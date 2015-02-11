#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <fstream> 
#include <math.h>
#include <string.h>

void putpixel(int x, int y, int** data){
	data[x][y] = 0;
}

int main() {
  // Define Complex Corners of Image
  int height = 2000;
  int width = 2000;
  float MinRe = -2.0;
  float MaxRe = 1.0;
  float MinIm = -1.2;
  float MaxIm = MinIm + (MaxRe-MinRe)*height/width;
  float Re_factor = (MaxRe - MinRe)/(width - 1);
  float Im_factor = (MaxIm - MinIm)/(height - 1);
  int maxIterations = 1024;
  int **img = new int*[height];
  for(int i =0; i < height; i++){
  	img[i] = new int[width];
  }
  char numstr[21];
  sprintf(numstr, "%d", maxIterations);
  std::cout << numstr << std::endl;
  char f_name[] = "man_seq_";
  char ppm[] = ".ppm";
  strcat(f_name,numstr);
  strcat(f_name,ppm);
  std::cout << f_name << std::endl;
  // sprintf(numstr, "%d",maxIterations);
  // f_name += numstr;
  // f_name +=std::to_string(maxIterations);
  std::ofstream out(f_name);

  // Initialize image with all white inputs
  for(int x = 0; x < width; x++){
  	for(int y = 0; y < height; y++){
  		img[x][y]= 255;
  	}
  }

  // Timing stuff
  struct timeval start, fin;

  // Map Complex Numbers to Pixels:
  gettimeofday(&start, NULL);
  for(unsigned y = 0; y < height; y++){
  	float c_im = MaxIm - y*Im_factor;
  	for(unsigned x=0; x<width; x++){
  		float c_re = MinRe + x*Re_factor;

  		float Z_re = c_re, Z_im = c_im;
  		bool isInside = true;
  		for(unsigned n = 0; n<maxIterations; n++){
  			float Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
  			if(Z_re2 + Z_im2 > 4){
  				isInside = false;
  				break;
  			}
  			Z_im = 2*Z_re*Z_im + c_im;
  			Z_re = Z_re2 - Z_im2 + c_re;
  		}
  		if(isInside) {putpixel(x,y,img);}
  	}
  }
  gettimeofday(&fin, NULL);


  double t = fin.tv_usec + fin.tv_sec * 1000000.0;
  t -= start.tv_usec + start.tv_sec * 1000000.0;
  t /= 1000000.0;
  std::cout << "Processing " << maxIterations << " iterations took " << t << " seconds" << std::endl;

  // Write to image:
  out << "P2" << '\n';
  out << "2000 2000" << '\n';
  out << "255" << '\n';

  for(int y = 0; y < width; y++){
  	for(int x = 0; x < height; x++){
  		out << img[x][y] << " ";
  	}
  	out << "\n";
  }

  delete img;
}	
