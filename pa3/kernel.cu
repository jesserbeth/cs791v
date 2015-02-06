
#include "kernel.h"

__global__ void mandel(int *out, int iterations, int minRe, int maxRe, 
                       int minIm, int maxIm, int rowSize, int colSize) {
  // int tid = threadIdx.x;
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  int y = tid / rowSize;
  int x = tid - rowSize*y;
  float Re_factor = (maxRe - minRe)/(rowSize - 1);
  float Im_factor = (maxIm - minIm)/(colSize - 1);

  // float Z_im = maxIm - y * Im_factor;
  // float Z_re = maxRe - x * Re_factor;
  float c_im = maxIm - y*Im_factor;
  float c_re = minRe + x*Re_factor;
  float Z_re = c_re, Z_im = c_im;
  bool inside = true;
  for(unsigned n=0; n < iterations; n++){
    float Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
    if(Z_re2 + Z_im2 > 4){
      inside = false;
      break;
    }
    Z_im = 2*Z_re*Z_im + Z_im;
    Z_re = Z_re2 - Z_im2 + Z_re;
  }

  if(inside == true && tid < (rowSize*colSize))
    out[tid] = 0;
}
