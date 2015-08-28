
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
   // float _timeN = INF;
   // bool update = true;
   // bool work = false;

   timeNow = times[1]; // timeNow = timeNext
   timeNext = INF;
   // printf("%d ", 5);

   while(cell < size){
    // printf("%d ", cell);
    // timeNext = INF;
      row = cell / rowSize;
      col = cell - rowSize*row;
      ignCell = ignTimeIn[cell];

      // Do atomic update of TimeNext Var (atomicMin)
      // if(timeNext > ignTimeIn[cell] && ignTimeIn[cell] > timeNow){
      if(timeNext > ignCell && ignCell > timeNow){
         atomicExch(&times[1], ignCell);
          // printf("%d ", 5);
         // times[1] = ignCell;
         timeNext = ignCell;
         // work = true;
         // printf("timeupdate\n");
      }
      else if(ignCell == timeNow){
      // if(ignCell == timeNow){
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
            // if(ignCellN == INF){
                // compute ignition time
                ROS = rothData[3*cell + 0] * (1.0 - rothData[3*cell + 1]) / 
                      (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180));

                float ignTimeNew = timeNow + L_n[n] / ROS;


                // printf("%f, %f, %f, %f \n", timeNow, L_n[n], ROS, ignTimeNew);

                // if(ignTimeNew < ignCellN){
                //   printf("test1: %d ", ignTimeNew);
                // }
                  float old = atomicExch(&ignTimeOut[ncell], ignTimeNew);
                  // Check to make sure we don't overwrite the past
                  if(old < ignTimeNew){
                    atomicExch(&ignTimeOut[ncell], old);
                  // printf("test2: %d \n", ignTimeNew);
                  }
                // if(ignTimeNew < ignTimeIn[ncell])
                //     ignTimeOut[ncell] = ignTimeNew; // This could cause a race cond.
                


                // }


                /*if(ignTimeNew < timeNext){
                // if(ignTimeNew < times[1]){
                  // update = false;
                  old = atomicExch(&times[1], ignTimeNew);
                  // timeNext = ignTimeNew;
                  
                  if(old < ignTimeNew && old > timeNext){
                  // if(old < ignTimeNew && old > timeNow){
                    atomicExch(&times[1], old);
                    printf("%d ", 1);
                    // timeNext = old;
                  }

                  // printf("cell: %d updated\n", cell);
                  // if(ignTimeNew < _timeN)
                    // _timeN = ignTimeNew; 
                  // printf("%f, %f \n \n", times[1], ignTimeNew);
                }*/
                // Local timeNext update
                if(ignTimeNew < timeNext){
                  timeNext = ignTimeNew;
                }
            }
         }
         // Check global timenext 
         float nextCheck = atomicExch(&times[1], timeNext);
                  // timeNext = ignTimeNew;
                  
          if(nextCheck < timeNext){
            atomicExch(&times[1], nextCheck);
            // printf("%d ", 1);
          }
         // work = true;
      }
                  // timeNext = _timeN;
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
   // timeNext = _timeN;
   // float newt = atomicExch(&times[1], timeNext);
   // if(newt < timeNext)
   //  atomicExch(&times[1], newt);

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
      if(fabs(ignCell - ignCellNew) < .00001f && ignCell != INF
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

            ROS = rothData[3*cell + 0] * (1.0f - rothData[3*cell + 1]) / 
                (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159f/180));
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
__global__ void BurnDist(float* ignTimeIn, float* ignTimeOut,float* burnDist,
                         float* rothData, float* times,float* L_n, int size,
                         int rowSize, int colSize, float timeStep, float t){
      /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   int nCol[8] =        {  0,  1,  1,  1,  0, -1, -1, -1};
   int nRow[8] =        {  1,  1,  0, -1, -1, -1,  0,  1};
   // printf("Iterative Minimal Time\n");
   float ignTime, ignTimeN;
   float dist;

   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   int ncell, nrow, ncol, row, col, distCell;
   float ROS;

      while(cell < size){
         row = cell / rowSize;
         col = cell - rowSize*row;
         // printf("%d ", cell);
         ignTime = ignTimeIn[cell];
         if(ignTime == INF){
            cell += blockDim.x * gridDim.x;
            continue;
          }

          // check neighbors for ignition
          for(int n = 0; n < 8; n++){
            // printf("%d ", n);
            distCell = cell * 8;
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
                (1.0 - rothData[3*cell + 1] * cos(rothData[3*cell + 2] * 3.14159/180.));

              // Burn distance
              dist = burnDist[distCell+n];
              dist = dist - ROS*timeStep;
              burnDist[distCell+n] = dist;

              // Propogate fire 
              if(dist <= 0){
                float old = atomicExch(&ignTimeOut[ncell], t);
                if(old < t)
                   atomicExch(&ignTimeOut[ncell], old);
                if(ncell == 0)
                  atomicAdd(&end, 1);
                if(ncell == (rowSize - 1))
                  atomicAdd(&end, 1);
                if(ncell == (nrow*rowSize-1))
                  atomicAdd(&end, 1);
                if(ncell == size -1)
                  atomicAdd(&end, 1);
              }

          }
         cell += blockDim.x * gridDim.x;
      }
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
__global__ void copyKernelBD(float* input, float* output, int size){
   // copy from output to input
   int cell = blockIdx.x * blockDim.x + threadIdx.x;
   // printf("%d ", cell);
   // end = 0;

   while(cell < size){
      input[cell] = output[cell];
      // if(check[cell] == true){
      //    // printf("true\n");
      //    atomicAdd(&end, 1);
      // }
      cell += blockDim.x * gridDim.x;
      // printf("%d ", end);
   }
   // printf("copy_out\n");
}
#endif // KERNEL_H_
