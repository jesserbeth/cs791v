
#include "kernel.h"
#include <math.h>

__global__ void mandel(int *out, int iterations, float minRe, float maxRe, 
                       float minIm, float maxIm, int rowSize, int colSize) {
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  if(tid < rowSize*colSize){
    unsigned y = tid / rowSize;
    unsigned x = tid - rowSize*y;
    float Re_factor = (maxRe - minRe)/(rowSize - 1);
    float Im_factor = (maxIm - minIm)/(colSize - 1);

    float c_im = maxIm - y*Im_factor;
    float c_re = minRe + x*Re_factor;
    float Z_re = c_re, Z_im = c_im;
    bool inside = true;
    for(unsigned n = 0; n<iterations; n++){
      float Z_re2 = Z_re*Z_re, Z_im2 = Z_im*Z_im;
      if(Z_re2 + Z_im2 > 4){
        inside = false;
        break;
      }
      Z_im = 2*Z_re*Z_im + c_im;
      Z_re = Z_re2 - Z_im2 + c_re;
    }
    if(inside == true)
      out[tid] = 0;
  }
}
