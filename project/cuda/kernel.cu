// #include "kernel.h"
// #include <math.h>
// #include <stdio.h>

// const int INF = 999999;

/////////////////////////////////////////////////////////////////////////////
//                              Minimal Time
/////////////////////////////////////////////////////////////////////////////
// __global__ void MinTime(float* ignTime, float* rothData, float* times, 
//                    float* L_n, int size, int rowSize, int colSize){
// __global__ void MT(){
   // // Get thread id + stride
   // /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   // int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   // int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   
   // int cell = blockIdx.x * blockDim.x + threadIdx.x;
   // int ncell, nrow, ncol, row, col;
   // float ignCell, ignCellN, timeNext, timeNow, ROS;
   // // timeNext = INF;
   // // get timenext and timenow from global memory

   // timeNow = times[1]; // timeNow = timeNext
   // timeNext = INF;
   // while(cell < size){
   //    row = cell / rowSize;
   //    col = cell - rowSize*row;
   //    // Load ignition cell to register
   //    ignCell = ignTime[cell];

   //    // Do atomic update of TimeNext Var (atomicMin)
   //    if(timeNext > ignTime[cell] && ignTime[cell] > timeNow){
   //       atomicExch(&times[1], ignCell);
   //       timeNext = ignCell;
   //    }
   //    else if(ignCell == timeNow){

   //       // Find burning cells
   //       for(int n = 0; n < 16; n++){
   //          // Propagate from burning cells      
   //          nrow = row + nRow[n];
   //          ncol = col + nCol[n];
   //          if ( nrow<0 || nrow>= rowSize || ncol<0 || ncol>=  colSize )
   //             continue;
   //          ncell = ncol + nrow*colSize;
   //          ignCellN = ignTime[ncell];
   //          // printf("%f ", ignCellN);

   //          // If neighbor is unburned
   //          if(ignCellN > timeNow){
   //              // compute ignition time
   //              ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
   //                    (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));
   //              // ROS = 10.0 * (1.0 - 0.321) / (1.0 - 0.321 * cos(30.f));
   //             // ROS = 0.5;
   //              float ignTimeNew = timeNow + L_n[n] / ROS;
   //              // printf("%f, %f, %f, %f \n", timeNow, L_n[n], ROS, ignTimeNew);
   //              if(ignTimeNew < ignCellN){
   //                // printf("%f ", ignTime[ncell]);
   //                  // ignTime[ncell] = ignTimeNew; // This could cause a race cond.
   //                // atomicMin(&ignTime[ncell], ignTimeNew);
   //                if(ignTimeNew < ignTime[ncell])
   //                   ignTime[ncell] = ignTimeNew;
   //                // float tmp = atomicExch(&ignTime[ncell], ignTimeNew);
   //                // if(tmp < ignTimeNew)
   //                //    atomicExch(&ignTime[ncell], ignTimeNew);
   //                  // printf("%f \n",ignTime[ncell]);
   //              }
   //              if(ignTimeNew < timeNext){
   //                  // timeNext = ignTimeNew;
   //                // printf("%f, %f \n", times[1], ignTimeNew);
   //                atomicExch(&times[1], ignTimeNew);
   //                // printf("%f, %f \n \n", times[1], ignTimeNew);
   //              }
   //          }
   //       }
   //    }

   //    // Do striding
   //    cell += blockDim.x * gridDim.x;
   //    // printf("%d \n", cell);
   // }
   // // printf("%f\n", timeNext);
   // if(timeNext == INF){
   //    // printf("BLAH");
//       end = 1;
//       printf("Kernel: %d\n", end);
//    // }
// }

// #endif


// #if IMT

// #endif


// #if BURNDIST

// #endif