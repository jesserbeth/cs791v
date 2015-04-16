#include <iostream>
#include <fstream>
#include "fireSim.h"
#include <sys/time.h>
#include "kernel.h"

// const int INF = 999999;

// __device__ int end;

// __global__ void MT(){

//   end = 1;
//   printf("Kernel: %d\n", end);
// }


int main(){
   for(int T = 64; T < 65; T<<=1){
      // Declare simulation variables
      int cell, row, col, nrow, ncol, ncell;
      // char simType[20];
      std::ofstream fout;

      // Initialize simulator
      fireSim sim(T,T);
      struct timeval start, fin;
      // float pi = 3.14159;
      // float ROS = 0;
      // float superSize = sqrt(pow(sim.cellSize, 2) + pow(sim.cellSize*2, 2));
      /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
      // static int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
      // static int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
      

    sim.init();
    // cout << "initialized" << endl;
    sim.updateSpreadData();

    // Allocate Roth Data for GPU
    // float* gpuRoth = new float[sim.simDimX*sim.simDimY*3];
    float* gpuRoth;
    gpuRoth = (float*)malloc(sim.simDimX*sim.simDimY*3*sizeof(float));
    float* gpuTime;
    gpuTime = (float*)malloc(sim.simDimX*sim.simDimY*sizeof(float));
    float* timeSteppers;
    timeSteppers = (float*)malloc(2*sizeof(float));
    float* loc_L_n;
    loc_L_n = (float*)malloc(16*sizeof(float));
    // cout << "hello" << endl;
    // for(int k = 0, c = 0; k < sim.simDimX*sim.simDimY*3; k+=3, c++){
    for(int k = 0, cell = 0, tcell = 0; k < sim.simDimX; k++){
      for(int c = 0; c < sim.simDimY; c++, cell+=3, tcell++){
        // cout << cell << endl;
        gpuRoth[cell + 0] = sim.rothData[k][c].x;
        gpuRoth[cell + 1] = sim.rothData[k][c].y;
        gpuRoth[cell + 2] = sim.rothData[k][c].z;
        gpuTime[tcell] = sim.ignTime[tcell];
        // cout << gpuTime[tcell] << " " <<sim.ignTime[tcell] << endl;
        // cout << gpuRoth[cell] <<" " << gpuRoth[cell+1] <<" " << gpuRoth[cell+2] << endl;
        // int ROS = gpuRoth[cell + 0] * (1.0 - gpuRoth[cell + 1]) / 
        //               (1.0 - gpuRoth[cell + 1] * cos(gpuRoth[cell + 2] * 3.14159/180));
        // cout << ROS << endl;
      }
    }
    // Allocate Time data for GPU 
    // float* timeSteppers = new float[2];
    // cout << "CPU: " << endl;
    for(int i = 0; i < 16; i++){
      loc_L_n[i] = sim.L_n[i];
    }
    timeSteppers[0] = 0;
    timeSteppers[1] = 0;


    char simType[20];
    sprintf(simType, "../out/GPU_DEBUG");

    // Allocate Cuda Variables
    float *g_ignTime;
    float* g_rothData;
    float* g_times;
    float* g_L_n;

    cudaError_t err = cudaMalloc( (void**) &g_ignTime, sim.simDimX*sim.simDimY*sizeof(float));
    err = cudaMalloc( (void**) &g_rothData, sim.simDimX*sim.simDimY*3*sizeof(float));
    err = cudaMalloc( (void**) &g_times, 2*sizeof(float));
    err = cudaMalloc( (void**) &g_L_n, 16*sizeof(float));
    if (err != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
        exit(1);
      }


    err = cudaMemcpy(g_ignTime, gpuTime, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_rothData, gpuRoth, sim.simDimX*sim.simDimY*3*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_times, timeSteppers, 2*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_L_n, loc_L_n, 16*sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
        exit(1);
    }

    // Kernel Loop
    int counter = 0;
    // cout << "Test" << endl;
    float t1, t2;
    // terminate = 0;
    cout << "Kicking off Kernels" << endl;
    typeof(end) terminate = -1;
    // while(terminate <= 0){
    while(counter < 561){
      counter++;

      // cout << counter << endl;
      MinTime<<<4,4>>>(g_ignTime, g_rothData, g_times, g_L_n, sim.simDimX*sim.simDimY,
                    sim.simDimX, sim.simDimY);
      // MT<<<1,1>>>();
      cudaDeviceSynchronize();
      err = cudaMemcpyFromSymbol(&terminate, end, sizeof(end), 0, 
                                 cudaMemcpyDeviceToHost);
      if (err != cudaSuccess) {
          std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
          exit(1);
      }
    }
      cout << counter <<endl;

    // Copy back to device
    err = cudaMemcpy(gpuTime, g_ignTime, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(gpuRoth, g_rothData, sim.simDimX*sim.simDimY*3*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(timeSteppers, g_times, 2*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(sim.L_n, g_L_n, 16*sizeof(float), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
        exit(1);
    }

      // Write data to file
      char threadNum[21];
      sprintf(threadNum, "_%d_%d", sim.simDimX, sim.simDimY);
      char csv[] = ".csv";
      strcat(simType,threadNum);
      strcat(simType,csv);
      fout.open(simType);
      for(int i = 0; i < sim.simDimX*sim.simDimY; i++){
        // std::cout << ignTime[i] << " ,";
        if(i %sim.simDimX == 0 && i !=0){
            // std::cout << std::endl;
            fout << '\n';
        }
        // fout << (int)sim.ignTime[i] << " ";
        // fout << (int)ignTimeNew[i] << " ";
        fout << (int) gpuTime[i] << " ";
      }
      fout.close();
      
   }

   return 0;
}