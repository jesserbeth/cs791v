
#ifndef KERNEL_H_
#define KERNEL_H_
#include <math.h>
#include <stdio.h>
__device__ int end;
__device__ int syncCounter;
const int INF = 999999;

/////////////////////////////////////////////////////////////////////////////
//                              Minimal Time
/////////////////////////////////////////////////////////////////////////////
__global__ void MinTime(float* ignTimeIn, float* ignTimeOut, float* rothData,
                        float* times,float* L_n, int size, int rowSize,
                        int colSize){
// __global__ void MT(){
   // Get thread id + stride
   /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   int ncell, nrow, ncol, row, col;
   float ignCell, ignCellN, timeNext, timeNow, ROS;
   // bool work = false;

   timeNow = times[1]; // timeNow = timeNext
   timeNext = INF;
   // printf("%d ", blockSize);

   while(cell < size){
      row = cell / rowSize;
      col = cell - rowSize*row;
      ignCell = ignTimeIn[cell];

      // Do atomic update of TimeNext Var (atomicMin)
      // if(timeNext > ignTimeIn[cell] && ignTimeIn[cell] > timeNow){
      if(timeNext > ignTimeIn[cell] && ignTimeIn[cell] > timeNow){
         atomicExch(&times[1], ignCell);
         timeNext = ignCell;
         // work = true;
         // printf("timeupdate\n");
      }
      else if(ignCell == timeNow){
         // Find burning cells
         for(int n = 0; n < 16; n++){
            // // Propagate from burning cells      
            nrow = row + nRow[n];
            ncol = col + nCol[n];
         // printf("nrow: %d ncol: %d\n",nrow,ncol);
            if ( nrow<0 || nrow>= rowSize || ncol<0 || ncol>=  colSize ){
               continue;
            }
            ncell = ncol + nrow*colSize;
            ignCellN = ignTimeIn[ncell];
            // ignCellN = neighbors[n];
            // printf("%f ", ignCellN);

            // If neighbor is unburned
            if(ignCellN > timeNow){
                // compute ignition time
                ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
                      (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));
                float ignTimeNew = timeNow + L_n[n] / ROS;
                // printf("%f, %f, %f, %f \n", timeNow, L_n[n], ROS, ignTimeNew);

                // if(ignTimeNew < ignCellN){
                // if(ignTimeNew < ignTimeOut[ncell]){
                    // ignTime[ncell] = ignTimeNew; // This could cause a race cond.
                float old = atomicExch(&ignTimeOut[ncell], ignTimeNew);
                if(old < ignTimeNew)
                   atomicExch(&ignTimeOut[ncell], old);
                // }
                if(ignTimeNew < timeNext){
                  atomicExch(&times[1], ignTimeNew);
                  // timeNext = ignTimeNew;
                  // printf("%f, %f \n \n", times[1], ignTimeNew);
                }
            }
         }
         // work = true;
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
   if(blockIdx.x * blockDim.x + threadIdx.x == 0)
      end = 0;

}

/////////////////////////////////////////////////////////////////////////////
//                          Iterative Minimal Time
/////////////////////////////////////////////////////////////////////////////
__global__ void ItMinTime(float* ignTimeIn, float* ignTimeOut, float* ignTimeStep,
                          float* rothData, float* times, float* L_n, bool* check, 
                          int size, int rowSize, int colSize){
   /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   // printf("Iterative Minimal Time\n");
   float ignCell = 0.;
   float ignCellNew = 0.;
   float ignTimeMin = INF;
   
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   int ncell, nrow, ncol, row, col;
   float ignTimeNew, ROS;

   while(cell < size){
      row = cell / rowSize;
      col = cell - rowSize*row;
      // printf("%d ", cell);
      if(check[cell] == true){
        cell += blockDim.x * gridDim.x; 
        // printf("if_statement_1\n");
        continue;
      }
      // Check for simulation completion
      ignCell = ignTimeIn[cell];
      ignCellNew = ignTimeOut[cell];
      // Convergence Test
      if(fabs(ignCell - ignCellNew) < .00001 && ignCell != INF
            && ignCellNew != INF && check[cell] != true){
        check[cell] = true;
        cell += blockDim.x * gridDim.x;  
        // printf("CONVERGED\n");
        continue;
      }

      if(ignCell > 0){
        // ignTimeMin = INF; 
        // printf("ignCell > 0\n");
        ignTimeMin = INF;
        // Loop through neighbors
        for(int n = 0; n < 16; n++){
            // find neighbor cell index     
            nrow = row + nRow[n];
            ncol = col + nCol[n];
            if ( nrow<0 || nrow>= rowSize || ncol<0 || ncol>=  colSize ){
               continue;
            }
            ncell = ncol + nrow*colSize;

            ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
                (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));
            ignTimeNew = ignTimeIn[ncell] + L_n[n] / ROS;
            // printf("ignTimeNew: %f ", ignTimeNew);
            ignTimeMin = ignTimeNew*(ignTimeNew < ignTimeMin) + ignTimeMin*(ignTimeNew >= ignTimeMin);
        }
        ignTimeStep[cell] = ignTimeMin;
      }
      cell += blockDim.x * gridDim.x;
   }
   // printf("Testing IMT Kernel\n");
   if(blockIdx.x * blockDim.x + threadIdx.x == 0)
      end = 0;
}


/////////////////////////////////////////////////////////////////////////////
//                            Burning Distances
/////////////////////////////////////////////////////////////////////////////
__global__ void BurnDist(float* ignTimeIn, float* ignTimeOut, float* rothData,
                        float* times,float* L_n, int size, int rowSize,
                        int colSize, float timeStep, float t){
      /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   // printf("Iterative Minimal Time\n");
   float ignCell = 0.;
   float ignCellNew = 0.;
   float ignTimeMin = INF;
   float ignTime, ignTimeN;
   float dist;

   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   int ncell, nrow, ncol, row, col;
   float ignTimeNew, ROS;



      int corner = 0;
      
      // float t = 0.0;
      // while(corner < 4){   
      //   for ( cell=0, row=0; row<sim.simDimX; row++ ){
      //       for ( col=0; col<colSize; col++, cell++ ){
                // check if already "ignited"
      while(cell < size){
         ignTime = ignTimeIn[cell];
         if(ignTime == INF){
            cell += blockDim.x * gridDim.x;
            continue;
          }
          // check neighbors for ignition
          for(int n = 0; n < 8; n++){
              nrow = row + nRow[n];
              ncol = col + nCol[n];
              if ( nrow<0 || nrow>=rowSize || ncol<0 || ncol>=colSize )
                  continue;
              ncell = ncol + nrow*colSize;

              // check for already lit
              ignTimeN = ignTimeIn[ncell];
              if(ignTimeN < INF){
                  continue;
              }
              // Calc roth values
              ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
                (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));

              // Burn distance
              dist = burnDist[ncell][n];
              dist = dist - rate*step;
              if(dist<0)
               dist = 0;
              // Propogate fire 
              if(dist == 0){
                 ignTimeOut[ncell] = t;
              }
              // sim.burnDist[ncell][n] = sim.burnDistance(sim.burnDist[ncell][n], 
              //                                           ROS,
              //                                           sim.timeStep);
              // // Propogate fire step:
              // if(sim.burnDist[ncell][n] == 0){
              //     sim.ignTimeNew[ncell] = t;
              //     if(nrow == (rowSize-1) && ncol == (colSize-1)){
              //         corner += 1;
              //     }
              //     if(nrow == 0 && ncol == (colSize-1)){
              //         corner += 1;
              //     }
              //     if(nrow == 0 && ncol == 0){
              //         corner += 1;
              //     }
              //     if(nrow == (rowSize-1) && ncol == 0){
              //         corner += 1;
              //     }
              // }
          }
         cell += blockDim.x * gridDim.x;
      }

      if(blockIdx.x * blockDim.x + threadIdx.x == 0)
         t += timeStep;
}




/////////////////////////////////////////////////////////////////////////////
//                             Copy Kernel (MT)
/////////////////////////////////////////////////////////////////////////////
__global__ void copyKernelMT(float* input, float* output, int size){
   // copy from output to input
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   // atomicExch(&end, 0);
   // end = 0;
   while(cell < size){
      input[cell] = output[cell];
      if(input[cell] != INF)
         atomicAdd(&end, 1);
      cell += blockDim.x * gridDim.x;
      // printf("%d ", end);
   }
}

/////////////////////////////////////////////////////////////////////////////
//                             Copy Kernel (IMT)
/////////////////////////////////////////////////////////////////////////////
__global__ void copyKernelIMT(float* input, float* output, bool* check, int size){
   // copy from output to input
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   // printf("%d ", cell);
   // end = 0;

   while(cell < size){
      input[cell] = output[cell];
      if(check[cell] == true){
         // printf("true\n");
         atomicAdd(&end, 1);
      }
      cell += blockDim.x * gridDim.x;
      // printf("%d ", end);
   }
}

/////////////////////////////////////////////////////////////////////////////
//                             Copy Kernel (BD)
/////////////////////////////////////////////////////////////////////////////
__global__ void copyKernelBD(float* input, float* output, bool* check, int size){
   // copy from output to input
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   // printf("%d ", cell);
   // end = 0;

   while(cell < size){
      input[cell] = output[cell];
      if(check[cell] == true){
         // printf("true\n");
         atomicAdd(&end, 1);
      }
      cell += blockDim.x * gridDim.x;
      // printf("%d ", end);
   }
}
#endif // KERNEL_H_
