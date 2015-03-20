#include <iostream>
#include "fireSim.h"


int main(){
   // Initialize simulator
   fireSim sim;
   sim.init();
   float startTime = 1.0;
   float endTime = 10000.0;
   float stepTime = 2.0;

   // Get data: right now it is test data

   // endTime = currentTime
   // startTime = lastUpdateTime
   while (sim.startTime != endTime){
      // stepTime = min(endTime, nextEvent.time)
      // UpdateCorruptedData()

      // sim.propogateFire(startTime, stepTime)
      sim.propagateFire();
      // BurnDistances(startTime, stepTime)

      // Accelerate(startTime, stepTime)

      // TriggerNextEvent()
      sim.startTime += stepTime;
   }

   std::cout << "printing values in array:" << std::endl;

   for(int i = 0; i < 10; i++){
      for(int j =0;j<10;j++){
         std::cout << sim.orthoSpreadRate[i][j].x << ", ";
         std::cout << sim.orthoSpreadRate[i][j].y << ", ";
         std::cout << sim.orthoSpreadRate[i][j].z << ", ";
         std::cout << sim.orthoSpreadRate[i][j].w << ", ";
         std::cout << sim.diagSpreadRate[i][j].x << ", ";
         std::cout << sim.diagSpreadRate[i][j].y << ", ";
         std::cout << sim.diagSpreadRate[i][j].z << ", ";
         std::cout << sim.diagSpreadRate[i][j].w << ", ";
         // std::cout << sim.timeOfArrival[i][j] << ", ";
      }
      std::cout<<std::endl;
   }
   // end while
   // lastUpdateTime = endTime
   return 0;
}