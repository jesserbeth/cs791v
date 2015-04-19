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

#define MT 1
#define IMT 0
#define BD 0

int main(){
  float memTime, calcTime;
  // int SIMTYPE = 1;
   for(int S = 2048; S < 2049; S<<=1){
      // Declare simulation variables
      int cell, row, col, nrow, ncol, ncell;
      // char simType[20];
      std::ofstream fout;

      // Initialize simulator
      fireSim sim(S,S);
      struct timeval start, fin;

    sim.init();
    sim.updateSpreadData();

    // Allocate Roth Data for GPU
    float* gpuRoth;
    gpuRoth = (float*)malloc(sim.simDimX*sim.simDimY*3*sizeof(float));
    float* gpuTime;
    gpuTime = (float*)malloc(sim.simDimX*sim.simDimY*sizeof(float));
    float* timeSteppers;
    timeSteppers = (float*)malloc(2*sizeof(float));
    float* loc_L_n;
    loc_L_n = (float*)malloc(16*sizeof(float));
    bool* check = (bool*)malloc(sim.simDimX*sim.simDimY*sizeof(bool));


    for(int k = 0, cell = 0, tcell = 0; k < sim.simDimX; k++){
      for(int c = 0; c < sim.simDimY; c++, cell+=3, tcell++){
        // cout << cell << endl;
        gpuRoth[cell + 0] = sim.rothData[k][c].x;
        gpuRoth[cell + 1] = sim.rothData[k][c].y;
        gpuRoth[cell + 2] = sim.rothData[k][c].z;
        gpuTime[tcell] = sim.ignTime[tcell];

        check[tcell] = false;
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

    gettimeofday(&start, NULL);
    float *g_ignTime_in;
    float *g_ignTime_out;
    float *g_ignTime_step;
    float *g_rothData;
    float *g_times;
    float *g_L_n;
    bool  *g_check;

    cudaError_t err = cudaMalloc( (void**) &g_ignTime_in, sim.simDimX*sim.simDimY*sizeof(float));
    err = cudaMalloc( (void**) &g_ignTime_out, sim.simDimX*sim.simDimY*sizeof(float));
    err = cudaMalloc( (void**) &g_rothData, sim.simDimX*sim.simDimY*3*sizeof(float));
    err = cudaMalloc( (void**) &g_times, 2*sizeof(float));
    err = cudaMalloc( (void**) &g_L_n, 16*sizeof(float));
#if IMT
    err = cudaMalloc( (void**) &g_check, sim.simDimX*sim.simDimY*sizeof(bool));
    err = cudaMalloc( (void**) &g_ignTime_step, sim.simDimX*sim.simDimY*sizeof(float));
#endif
    if (err != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
        exit(1);
      }

    err = cudaMemcpy(g_ignTime_in, gpuTime, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_ignTime_out, gpuTime, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_rothData, gpuRoth, sim.simDimX*sim.simDimY*3*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_times, timeSteppers, 2*sizeof(float), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_L_n, loc_L_n, 16*sizeof(float), cudaMemcpyHostToDevice);
#if IMT
    err = cudaMemcpy(g_check, check, sim.simDimX*sim.simDimY*sizeof(bool), cudaMemcpyHostToDevice);
    err = cudaMemcpy(g_ignTime_step, gpuTime, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyHostToDevice);
#endif
    if (err != cudaSuccess) {
        std::cerr << "Error: " << cudaGetErrorString(err) << std::endl;
        exit(1);
    }

    // Kernel Loop
    int counter = 0;
    // terminate = 0;
    cout << "Kicking off Kernels" << endl;
    typeof(syncCounter) terminate = -1;
    int B = 2048;
    int T = 512;
#if BD
    float t = 0.0;
#endif
    while(terminate <= 0){
    // while(counter < 200){
      counter++;
#if MT
        // Do calculations
        MinTime<<<B,T>>>(g_ignTime_in, g_ignTime_out, g_rothData, 
                         g_times, g_L_n, sim.simDimX*sim.simDimY,
                         sim.simDimX, sim.simDimY);
        // cout << "step caclulated\n";
        // Copy from output to write
        copyKernelMT<<<B,T>>>(g_ignTime_in, g_ignTime_out, 
                              sim.simDimX*sim.simDimY);

        cudaDeviceSynchronize();
        err = cudaMemcpyFromSymbol(&terminate, end, sizeof(end), 0, 
                                   cudaMemcpyDeviceToHost);
        // err = cudaMemcpyFromSymbol(&terminate, syncCounter, sizeof(syncCounter), 0, 
        //                            cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
            exit(1);
        }
        // cout << terminate <<endl;
        if(terminate < sim.simDimX*sim.simDimY)
          terminate = -1;
        cout << counter <<endl;
        
#endif
#if IMT
      // ITERATIVE MINIMAL TIME
        // Do calculations
        ItMinTime<<<B,T>>>(g_ignTime_in,g_ignTime_out, g_ignTime_step, g_rothData, 
                           g_times, g_L_n, g_check, sim.simDimX*sim.simDimY,
                           sim.simDimX, sim.simDimY);
        // cout << "step caclulated\n";
        // Copy from output to write
        copyKernelIMT<<<B,T>>>(g_ignTime_in, g_ignTime_step,
                            g_check, sim.simDimX*sim.simDimY);

        cudaDeviceSynchronize();
        // for(int k = 0; k < sim.simDimX*sim.simDimY; k++)
        //   cout << check[k] <<  " ";
        // cout << endl;
        err = cudaMemcpyFromSymbol(&terminate, end, sizeof(end), 0, 
                                   cudaMemcpyDeviceToHost);
        // err = cudaMemcpyFromSymbol(&terminate, syncCounter, sizeof(syncCounter), 0, 
        //                            cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
            exit(1);
        }
        // cout << terminate <<endl;
        if(terminate < sim.simDimX*sim.simDimY)
          terminate = -1;
        // cout << counter <<endl;
        // Swap Pointers for loop
        float *swap = g_ignTime_in;
        g_ignTime_in = g_ignTime_out;
        g_ignTime_out = swap;

#endif
#if BD
        // BURN ALL THE DISTANCES!!!!
        // Do calculations
        BurnDist<<<B,T>>>(g_ignTime_in,g_ignTime_out, g_ignTime_step, g_rothData, 
                           g_times, g_L_n, g_check, sim.simDimX*sim.simDimY,
                           sim.simDimX, sim.simDimY, sim.timeStep, t);
        // cout << "step caclulated\n";
        // Copy from output to write
        copyKernelBD<<<B,T>>>(g_ignTime_in, g_ignTime_step,
                            g_check, sim.simDimX*sim.simDimY);

        cudaDeviceSynchronize();
        // for(int k = 0; k < sim.simDimX*sim.simDimY; k++)
        //   cout << check[k] <<  " ";
        // cout << endl;
        err = cudaMemcpyFromSymbol(&terminate, end, sizeof(end), 0, 
                                   cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
            exit(1);
        }
        // cout << terminate <<endl;
        if(terminate < sim.simDimX*sim.simDimY)
          terminate = -1;
#endif
    }
    // cudaEventRecord(end, 0);
    // cudaEventSynchronize(end);

    // cudaEventElapsedTime( &calcTime, start, end);

    // Copy back to device
    err = cudaMemcpy(gpuTime, g_ignTime_in, sim.simDimX*sim.simDimY*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(gpuRoth, g_rothData, sim.simDimX*sim.simDimY*3*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(timeSteppers, g_times, 2*sizeof(float), cudaMemcpyDeviceToHost);
    // err = cudaMemcpy(sim.L_n, g_L_n, 16*sizeof(float), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        std::cerr << "Error copying from GPU: " << cudaGetErrorString(err) << std::endl;
        exit(1);
    }

      // cudaEventRecord(m_end, 0);
      // cudaEventSynchronize(m_end);

      // cudaEventElapsedTime(&memTime, m_start, m_end);

      gettimeofday(&fin, NULL);

      double t_init = fin.tv_usec + fin.tv_sec * 1000000.0;
      t_init -= start.tv_usec + start.tv_sec * 1000000.0;
      t_init /= 1000000.0;   
      std::cout << "Processing init on " << sim.simDimX << " cells took " << t_init << " seconds" << std::endl;
      
    // cudaEventDestroy( start );
    // cudaEventDestroy( end );
    // cudaEventDestroy( m_start );
    // cudaEventDestroy( m_end );



      // Write data to file
      char threadNum[21];
      sprintf(threadNum, "_%d_%d", sim.simDimX, sim.simDimY);
      char csv[] = ".csv";
      strcat(simType,threadNum);
      strcat(simType,csv);
      fout.open(simType);
      printf("Using %d Blocks and %d Threads with %d Iterations\n", B,T,counter);
      printf("Writing to %s\n", simType);
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