#ifndef KERNEL_H_
#define KERNEL_H_
#include <math.h>
#include <stdio.h>
__device__ int end;
const int INF = 999999;

/////////////////////////////////////////////////////////////////////////////
//                              Minimal Time
/////////////////////////////////////////////////////////////////////////////
__global__ void MinTime(float* ignTime, float* rothData, float* times, 
                   float* L_n, int size, int rowSize, int colSize){
// __global__ void MT(){
   // Get thread id + stride
   /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   int ncell, nrow, ncol, row, col;
   float ignCell, ignCellN, timeNext, timeNow, ROS;
   bool work = false;
   // timeNext = INF;
   // get timenext and timenow from global memory

   timeNow = times[1]; // timeNow = timeNext
   timeNext = INF;
   while(cell < size){
      row = cell / rowSize;
      col = cell - rowSize*row;
      // Load ignition cell to register
      ignCell = ignTime[cell];

      // Do atomic update of TimeNext Var (atomicMin)
      if(timeNext > ignTime[cell] && ignTime[cell] > timeNow){
         atomicExch(&times[1], ignCell);
         timeNext = ignCell;
         work = true;
      }
      else if(ignCell == timeNow){

         // Find burning cells
         for(int n = 0; n < 16; n++){
            // Propagate from burning cells      
            nrow = row + nRow[n];
            ncol = col + nCol[n];
            if ( nrow<0 || nrow>= rowSize || ncol<0 || ncol>=  colSize )
               continue;
            ncell = ncol + nrow*colSize;
            ignCellN = ignTime[ncell];
            // printf("%f ", ignCellN);

            // If neighbor is unburned
            if(ignCellN > timeNow){
            // if(ignCellN == INF){
                // compute ignition time
                ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
                      (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));
                float ignTimeNew = timeNow + L_n[n] / ROS;
                // printf("%f, %f, %f, %f \n", timeNow, L_n[n], ROS, ignTimeNew);

                if(ignTimeNew < ignCellN){
                    ignTime[ncell] = ignTimeNew; // This could cause a race cond.
                    // atomicExch(&ignTime[ncell], ignTimeNew);
                }
                if(ignTimeNew < timeNext){
                  atomicExch(&times[1], ignTimeNew);
                  timeNext = ignTimeNew;
                  // printf("%f, %f \n \n", times[1], ignTimeNew);
                }
            }
         }
         work = true;
      }
	  // if(timeNext == INF && work == true){
	  // // if(timeNext == INF){
	  //     // printf("BLAH");
	  //     end = 1;
	  //     // printf("Kernel: %d\n", end);
	  //  }

      // Do striding
      cell += blockDim.x * gridDim.x;
      // printf("%d \n", cell);
   }
}

// __global__ void MinTime(float*, float*, float*, float*, int, int, int);
// __global__ void MT();

#endif // KERNEL_H_
