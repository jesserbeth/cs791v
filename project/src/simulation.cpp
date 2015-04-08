#include <iostream>
#include "fireSim.h"


int main(){
   // Initialize simulator
   fireSim sim;
   sim.init();
   float startTime = 1.0;
   float endTime = 10.0;
   float stepTime = 2.0;

   // Get data: right now it is test data

   // endTime = currentTime
   // startTime = lastUpdateTime
   int counter = 0;
   // while (sim.startTime != endTime){
      // counter++;
      // stepTime = min(endTime, nextEvent.time)
      // UpdateCorruptedData()

      // sim.propogateFire(startTime, stepTime)
      // sim.propagateFire();
      sim.updateSpreadData();
      // BurnDistances(startTime, stepTime)

      // Accelerate(startTime, stepTime)

      // TriggerNextEvent()
      sim.startTime += stepTime;
   // }

   std::cout << "printing values in array:" << std::endl;

   // for(int i = 0; i < 10; i++){
   //    for(int j =0;j<10;j++){
         // std::cout << sim.orthoSpreadRate[i][j].x << ", ";
         // std::cout << sim.orthoSpreadRate[i][j].y << ", ";
         // std::cout << sim.orthoSpreadRate[i][j].z << ", ";
         // std::cout << sim.orthoSpreadRate[i][j].w << ", ";
         // std::cout << sim.diagSpreadRate[i][j].x << ", ";
         // std::cout << sim.diagSpreadRate[i][j].y << ", ";
         // std::cout << sim.diagSpreadRate[i][j].z << ", ";
         // std::cout << sim.diagSpreadRate[i][j].w << ", ";
         // std::cout << sim.timeOfArrival[i][j] << ", ";
   //       std::cout << sim.rothData[i][j].x << ", " <<sim.rothData[i][j].y << ", " <<sim.rothData[i][j].z << ", ";

   //    }
   //    std::cout<<std::endl;
   // }
   int numMoistModels = sim._moistures.size();
   for(int i = 0; i < numMoistModels; i++){
      // std::cout << sim.dead1hBuffer[i].x <<
      //              sim.dead1hBuffer[i].y <<
      //              sim.dead1hBuffer[i].z << std::endl;
   }
   // end while
   // lastUpdateTime = endTime
   // delete sim;
   return 0;
}