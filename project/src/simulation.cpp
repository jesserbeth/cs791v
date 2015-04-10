#include <iostream>
#include <fstream>
#include "fireSim.h"

const int INF = 9999999;

int main(){
   // Declare simulation variables
   int cell, row, col, nrow, ncol, ncell;
   std::ofstream fout;

   float cellSize = 30;
   float orthoSize = cellSize;
   float diagSize = cellSize * sqrt(2);
   float superSize = sqrt(pow(cellSize, 2) + pow(cellSize*2, 2));
   float pi = 3.14159;
   float ROS = 0;
   /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
   static int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
   static int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
   // static float angle[16] =     {   90, 45, 0, 45, 90, 45, 0, 45, 
                                  // 26.57, 63.43, 26.57, 26.57, 63.43, 63.43, 26.57, 26.57};
                                  // 27, 63, 27, 63, 27, 63, 27, 63};
   static float angle[16] =     {   30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30};
   static float L_n[16] =        { orthoSize, diagSize, orthoSize, diagSize, orthoSize, diagSize,
                                 orthoSize, diagSize, superSize,superSize,superSize,superSize,
                                 superSize,superSize,superSize,superSize};


   // Initialize simulator
   fireSim sim;
   sim.init();
   sim.updateSpreadData();
   // float startTime = 1.0;
   // float endTime = 10.0;
   float stepTime = 2.0;

   // Get data: right now it is test data

   // // endTime = currentTime
   // // startTime = lastUpdateTime
   // // int counter = 0;
   // // while (sim.startTime != endTime){
   //    // counter++;
   //    // stepTime = min(endTime, nextEvent.time)
   //    // UpdateCorruptedData()

   //    // sim.propogateFire(startTime, stepTime)
   //    // sim.propagateFire();
   //    sim.updateSpreadData();
   //    // BurnDistances(startTime, stepTime)

   //    // Accelerate(startTime, stepTime)

   //    // TriggerNextEvent()
   //    sim.startTime += stepTime;
   // // }
   int counter = 0;
    while(sim.timeNext < INF){
        sim.timeNow = sim.timeNext;
        sim.timeNext = INF;
        counter++;

        // Loop through all cells
        // for(cell = 0; cell < CELLS; cell++){
        for ( cell=0, row=0; row<sim.simDimX; row++ ){
            for ( col=0; col<sim.simDimY; col++, cell++ ){
                if(sim.timeNext > sim.ignTime[cell] && sim.ignTime[cell] > sim.timeNow){
                    sim.timeNext = sim.ignTime[cell];
                }
                else if( sim.ignTime[cell] == sim.timeNow){
                    for(int n = 0; n < 16; n++){
                    // for(int n = 0; n < 8; n++){
                        // find neighbor cell index
                        nrow = row + nRow[n];
                        ncol = col + nCol[n];
                        // std::cout << row << ' ' << col << ' ' << std::endl;
                        if ( nrow<0 || nrow>= sim.simDimX || ncol<0 || ncol>=  sim.simDimY )
                            continue;
                        ncell = ncol + nrow*sim.simDimY;

                        // If neighbor is unburned
                        if(sim.ignTime[ncell] > sim.timeNow){
                            // compute ignition time
                            ROS = sim.rothData[row][col].x * (1.0 - sim.rothData[row][col].y) / 
                                  (1.0 - sim.rothData[row][col].y * cos(sim.rothData[row][col].z * pi/180));
                            // std::cout << ROS << std::endl;
                            float ignTimeNew = sim.timeNow + L_n[n] / ROS;

                            if(ignTimeNew < sim.ignTime[ncell]){
                                sim.ignTime[ncell] = ignTimeNew;
                            }
                            if(ignTimeNew < sim.timeNext){
                                sim.timeNext = ignTimeNew;
                            }
                        }
                    }
                }
            }
        }
        // if(counter % 1000 == 0)
            // std::cout << counter << std::endl;
        std::cout << counter << endl;
    }

   // std::cout << "printing values in array:" << std::endl;
   // int cell = 0; 
   // for(int i = 0; i < sim.simDimX; i++){
   //    for(int j =0;j<sim.simDimY;j++, cell++){
   //       // std::cout << sim.orthoSpreadRate[i][j].x << ", ";
   //       // std::cout << sim.orthoSpreadRate[i][j].y << ", ";
   //       // std::cout << sim.orthoSpreadRate[i][j].z << ", ";
   //       // std::cout << sim.orthoSpreadRate[i][j].w << ", ";
   //       // std::cout << sim.diagSpreadRate[i][j].x << ", ";
   //       // std::cout << sim.diagSpreadRate[i][j].y << ", ";
   //       // std::cout << sim.diagSpreadRate[i][j].z << ", ";
   //       // std::cout << sim.diagSpreadRate[i][j].w << ", ";
   //       // std::cout << sim.timeOfArrival[i][j] << ", ";
   //       std::cout << sim.slopeAspectElevationTexture[cell].x << ", "; 
   // //       std::cout << sim.rothData[i][j].x << ", " <<sim.rothData[i][j].y << ", " <<sim.rothData[i][j].z << ", ";

   //    }
   //    std::cout<<std::endl;
   // }
   // int numMoistModels = sim._moistures.size();
   // for(int i = 0; i < numMoistModels; i++){
   //    // std::cout << sim.dead1hBuffer[i].x <<
   //    //              sim.dead1hBuffer[i].y <<
   //    //              sim.dead1hBuffer[i].z << std::endl;
   // }
   // end while
   // lastUpdateTime = endTime
   // delete sim;
   std::cout << "End of Simulation" << std::endl;


   // Write data to file

   char threadNum[21];
   sprintf(threadNum, "_%d", sim.simDimX);
   // char f_name[] = "data_MT";
   char f_name[] = "../out/test";
   char csv[] = ".csv";
   strcat(f_name,threadNum);
   strcat(f_name,csv);
   fout.open(f_name);

   for(int i = 0; i < sim.simDimX*sim.simDimY; i++){
     // std::cout << ignTime[i] << " ,";
     if(i %sim.simDimX == 0 && i !=0){
         // std::cout << std::endl;
         fout << '\n';
     }
     fout << (int)sim.ignTime[i] << " ";
     // fout << (int)ignTimeNew[i] << " ";
   }
   fout.close();

   return 0;
}