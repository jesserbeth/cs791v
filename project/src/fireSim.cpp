#include "fireSim.h"
#include <algorithm>
#include <math.h>


/*
Constructor: builds simplest test case for testing code
*/
fireSim::fireSim(){
   std::cout << "Initializing Simulation to Test Setting" << std::endl;

   // declare 2d map data
   int sizeX = 10;
   int sizeY = 10;

   simDimX = sizeX;
   simDimY = sizeY;

   timeOfArrival = new float*[simDimX];
   fuelTexture = new float*[simDimX];
   originalTimeOfArrival = new float*[simDimX];
   orthoSpreadRate = new vec4*[simDimX];
   diagSpreadRate = new vec4*[simDimX];
   orthoMaxSpreadRate = new vec4*[simDimX];
   diagMaxSpreadRate = new vec4*[simDimX];
   orthoBurnDistance = new vec4*[simDimX];
   diagBurnDistance = new vec4*[simDimX];
   updateStamp = new int*[simDimX];
   sourceDataTexture = new point*[simDimX];

   crownThreshold = new float*[simDimX];
   crownActiveRate = new float*[simDimX];
   canopyHeight = new float*[simDimX];
   spreadData = new float*[simDimX];

   for(int i = 0; i < simDimX; i++){
      timeOfArrival[i] = new float[simDimY];
      fuelTexture[i] = new float[simDimY];
      originalTimeOfArrival[i] = new float[simDimY];
      orthoSpreadRate[i] = new vec4[simDimY];
      diagSpreadRate[i] = new vec4[simDimY];
      orthoMaxSpreadRate[i] = new vec4[simDimY];
      diagMaxSpreadRate[i] = new vec4[simDimY];
      orthoBurnDistance[i] = new vec4[simDimY];
      diagBurnDistance[i] = new vec4[simDimY];
      updateStamp[i] = new int[simDimY];
      sourceDataTexture[i] = new point[simDimY];

      crownThreshold[i] = new float[simDimY];
      crownActiveRate[i] = new float[simDimY];
      canopyHeight[i] = new float[simDimY];
      spreadData[i] = new float[simDimY];
   }

   startTime = 0.; 
   baseTime = 0.;
   endTime = 1000.;
   lastStamp = 0.;
   currentStamp = 0.;
   accelerationConstant = 1.0;

   // outputTOA;
   // outputOrthoRates;
   // outputDiagRates;
   // timeStamp;
   // outputSourceData; 
}

/*
Destructor: builds simplest test case for testing code
*/
fireSim::~fireSim(){
   // delete all memory: need to be more clever with more complex sims
   int sizeX = 100;
   int sizeY = 100;

   // blah blah

}



/*
Function: Init
Input: TBD
Shader base: rothermel
Purpose: Initializes the sim. 
*/
void fireSim::init(){
   for(int i = 0; i < simDimX; i++){
      for(int j = 0; j < simDimY; j++){
         timeOfArrival[i][j] = 20.;
         fuelTexture[i][j] = 20.;
         originalTimeOfArrival[i][j] = 20.;
         orthoSpreadRate[i][j].x = orthoSpreadRate[i][j].y = orthoSpreadRate[i][j].z = orthoSpreadRate[i][j].w = 1.;
         diagSpreadRate[i][j].x = diagSpreadRate[i][j].y = diagSpreadRate[i][j].z = diagSpreadRate[i][j].w = 1.;
         orthoMaxSpreadRate[i][j].x =orthoMaxSpreadRate[i][j].y = orthoMaxSpreadRate[i][j].z = orthoMaxSpreadRate[i][j].w = 100.;
         diagMaxSpreadRate[i][j].x = diagMaxSpreadRate[i][j].y = diagMaxSpreadRate[i][j].z = diagMaxSpreadRate[i][j].w = 100.;
         orthoBurnDistance[i][j].x = orthoBurnDistance[i][j].y = orthoBurnDistance[i][j].z = orthoBurnDistance[i][j].w = .2;
         diagBurnDistance[i][j].x = diagBurnDistance[i][j].y = diagBurnDistance[i][j].z = diagBurnDistance[i][j].w = .2;
         updateStamp[i][j] = 0.;
         sourceDataTexture[i][j].x = sourceDataTexture[i][j].y = 0.;

         crownThreshold[i][j] = 0.0;
         crownActiveRate[i][j] = 100000.;
         canopyHeight[i][j] = 0.;
         spreadData[i][j] = 0.;
      }
   }
   spreadData[5][5] = 100;
   // cout << "HERE" << endl;
}

/*
Function: updateSpreadData
Input: TBD
Shader base: rothermel
Purpose: This runs rothermel's equations to initialize simulation
*/
void fireSim::updateSpreadData(){
   // This is where rothermel's shader is implemented
   
   for(int i = 0; i < simDimX; i++){
      for(int j = 0; j < simDimY; j++){
         // Shader code: int fuelModel = texture2D(fuelTexture, gl_TexCoord[1].st).r;
            // gl_TexCoord[1].st corresponds to fuelTexture.xy
         int fuelModel = fuelTexture[i][j];
      }
   }
   
}

/*
Function: propagateFire
Input: TBD
Shader base: propagateAccel
Purpose: 
*/
void fireSim::propagateFire(){
   // must loop through all points in lattice
   for(int i = 0; i < simDimX; i++){ // loop through rows
      for(int j = 0; j < simDimY; j++){ // loop through cols
         point n,s,e,w,nw,sw,ne,se;
         bool toprow = false;
         bool bottomrow = false;
         bool leftcol = false;
         bool rightcol = false;
         
         float toa = timeOfArrival[i][j];
         // cout << "before cont" << endl;
         if(toa <= startTime)
            continue;
         // cout << "after cont" << endl;
         float sourceData[4] = {sourceDataTexture[i][j].x, sourceDataTexture[i][j].y, i, j};
         point sourceDir;
         sourceDir.x = sourceData[0];
         sourceDir.y = sourceData[1];
         float originalToa = originalTimeOfArrival[i][j];
         float* orthoBurnDistances;
         orthoBurnDistances = new float[4];
         orthoBurnDistances[0] = orthoBurnDistance[i][j].x;
         orthoBurnDistances[1] = orthoBurnDistance[i][j].y;
         orthoBurnDistances[2] = orthoBurnDistance[i][j].z;
         orthoBurnDistances[3] = orthoBurnDistance[i][j].w;
         float* diagBurnDistances;
         diagBurnDistances = new float[4];
         diagBurnDistances[0] = diagBurnDistance[i][j].x;
         diagBurnDistances[1] = diagBurnDistance[i][j].y;
         diagBurnDistances[2] = diagBurnDistance[i][j].z;
         diagBurnDistances[3] = diagBurnDistance[i][j].w;

         // check x bounds
         if(i-1 >= 0){
            nw.x = i-1;
            n.x = i-1;
            ne.x = i-1;
            toprow = true;
         }
         else{
            nw.x = 0;
            n.x = 0;
            ne.x = 0;
            toprow = false;
         }
         if(i+1 < simDimX){
            sw.x = i+1;
            s.x = i+1;
            se.x = i+1;
            bottomrow = true;
         }
         else{
            sw.x = 0;
            s.x = 0;
            se.x = 0;
            bottomrow = false;
         }
         w.x = i;
         e.x = i;
         // check y bounds
         if(j-1 >=0){
            nw.y = j-1;
            w.y = j-1;
            sw.y = j-1;
            leftcol = true;
         }
         else{
            nw.y = 0;
            w.y = 0;
            sw.y = 0;
            leftcol = false;
         }
         if(j+1 < simDimY){
            ne.y = j+1;
            e.y = j+1;
            se.y = j+1;
            rightcol = true;
         }
         else{
            ne.y = 0;
            e.y = 0;
            se.y = 0;
            rightcol = false;

         }
         n.y = j;
         s.y = j;
         // if(toprow == true){
         //    bool updatenw = updateStamp[nw.x][nw.y] == lastStamp;
         //    bool updaten = updateStamp[n.x][n.y] == lastStamp;
         //    bool updatene = updateStamp[ne.x][ne.y] == lastStamp;
         // }
         // if(bottomrow == true){
         //    bool updatesw = updateStamp[sw.x][sw.y] == lastStamp;
         //    bool updates = updateStamp[s.x][s.y] == lastStamp;
         //    bool updatese = updateStamp[se.x][se.y] == lastStamp;
         // }
         // if(leftcol == true)
         //    bool updatew = updateStamp[s.x][s.y] == lastStamp;
         // if(rightcol == true)
         //    bool updatese = updateStamp[se.x][se.y] == lastStamp;

         // check if any updates necessary
         // if(!(updatenw | updaten | updatene | updatew | updatee | updatesw | updates | updatese))
         //    continue;

         bool toaCorrupt = updateStamp[sourceDir.x][sourceDir.y] == lastStamp;
         float newToa = toa;
         float toaLimit = toa;
         if(toaCorrupt || lastStamp == 0){
            newToa = originalToa;
            toaLimit = originalToa;
         }
         // REMEMBER THAT X and Y correspond to pointCoord!!
         point direction;
         direction.x = i;
         direction.y = j;
         float newRate = 0.0;
         float dt = 0.0;

         // check for boundaries as you propagate
         // if(toprow == true){
            // update NW
            {
               float t = timeOfArrival[nw.x][nw.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = diagSpreadRate[nw.x][nw.y].w;
                  float dist = diagBurnDistances[0];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = nw.x;
                     direction.y = nw.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update N
            {
               float t = timeOfArrival[n.x][n.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = orthoSpreadRate[n.x][n.y].w;
                  float dist = orthoBurnDistances[0];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = n.x;
                     direction.y = n.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update NE
            {
               float t = timeOfArrival[ne.x][ne.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = diagSpreadRate[ne.x][ne.y].z;
                  float dist = diagBurnDistances[1];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = ne.x;
                     direction.y = ne.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
         // }
         // if()
            // Update W
            {
               float t = timeOfArrival[w.x][w.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = orthoSpreadRate[w.x][w.y].z;
                  float dist = orthoBurnDistances[1];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = w.x;
                     direction.y = w.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update E
            {
               float t = timeOfArrival[e.x][e.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = orthoSpreadRate[e.x][e.y].y;
                  float dist = orthoBurnDistances[2];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = e.x;
                     direction.y = e.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update SW
            {
               float t = timeOfArrival[sw.x][sw.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = diagSpreadRate[sw.x][sw.y].y;
                  float dist = diagBurnDistances[2];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = sw.x;
                     direction.y = sw.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update S
            {
               float t = timeOfArrival[s.x][s.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = orthoSpreadRate[s.x][s.y].x;
                  float dist = orthoBurnDistances[3];
                  float burnTime = dist / rate;
                  t += burnTime;
                  if(t < newToa){
                     newToa = t;
                     direction.x = s.x;
                     direction.y = s.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }
            // Update SE
               // cout << "OUT" << endl;
            {
               float t = timeOfArrival[se.x][se.y];
               if(t < endTime){
                  t = max(baseTime, t);
                  float rate = diagSpreadRate[se.x][se.y].x;
                  float dist = diagBurnDistances[3];
                  float burnTime = dist / rate;
                  t += burnTime;
                  // cout << t << endl;
                  if(t < newToa){
                     newToa = t;
                     direction.x = se.x;
                     direction.y = se.y;
                     dt = burnTime;
                     newRate = rate;
                  }
               }
            }

            // if(newToa >= toaLimit || newToa > endTime)
            //    continue;
            // cout << "test" << endl;
            float maxOrthoRates[4] = {orthoMaxSpreadRate[i][j].x,
                                      orthoMaxSpreadRate[i][j].y,
                                      orthoMaxSpreadRate[i][j].z,
                                      orthoMaxSpreadRate[i][j].w};
            float maxDiagRates[4] = {diagMaxSpreadRate[i][j].x,
                                     diagMaxSpreadRate[i][j].y,
                                     diagMaxSpreadRate[i][j].z,
                                     diagMaxSpreadRate[i][j].w};
            float* currentOrthoRates;
            currentOrthoRates = new float[4];
            currentOrthoRates[0] = diagSpreadRate[direction.x][direction.y].x;
            currentOrthoRates[1] = diagSpreadRate[direction.x][direction.y].y;
            currentOrthoRates[2] = diagSpreadRate[direction.x][direction.y].z;
            currentOrthoRates[3] = diagSpreadRate[direction.x][direction.y].w;

            float* currentDiagRates;
            currentDiagRates = new float[4];
            currentDiagRates[0] = diagSpreadRate[direction.x][direction.y].x;
            currentDiagRates[1] = diagSpreadRate[direction.x][direction.y].y;
            currentDiagRates[2] = diagSpreadRate[direction.x][direction.y].z;
            currentDiagRates[3] = diagSpreadRate[direction.x][direction.y].w;

            float _canopyHeight = canopyHeight[i][j];
            if(_canopyHeight > 0.0){
               float _crownActiveRate = crownActiveRate[i][j];
               float _crownThreshold = crownThreshold[i][j];
               float intensityModifier = spreadData[i][j];
               // Ortho Rates
               maxOrthoRates[0] = testCrownRate(currentOrthoRates[0],
                                                maxOrthoRates[0],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxOrthoRates[1] = testCrownRate(currentOrthoRates[1],
                                                maxOrthoRates[1],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxOrthoRates[2] = testCrownRate(currentOrthoRates[2],
                                                maxOrthoRates[2],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxOrthoRates[3] = testCrownRate(currentOrthoRates[3],
                                                maxOrthoRates[3],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               // Diag Rates
               maxDiagRates[0] = testCrownRate(currentDiagRates[0],
                                                maxDiagRates[0],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxDiagRates[1] = testCrownRate(currentDiagRates[1],
                                                maxDiagRates[1],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxDiagRates[2] = testCrownRate(currentDiagRates[2],
                                                maxDiagRates[2],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
               maxDiagRates[3] = testCrownRate(currentDiagRates[3],
                                                maxDiagRates[3],
                                                intensityModifier,
                                                _crownActiveRate,
                                                _crownThreshold);
            }

            currentOrthoRates = accelerate(currentOrthoRates, maxOrthoRates, dt);
            currentDiagRates = accelerate(currentDiagRates, maxDiagRates, dt);

            // Write results
            timeOfArrival[i][j] = newToa;
            diagSpreadRate[i][j].x = currentDiagRates[0];
            diagSpreadRate[i][j].y = currentDiagRates[1];
            diagSpreadRate[i][j].z = currentDiagRates[2];
            diagSpreadRate[i][j].w = currentDiagRates[3];
            orthoSpreadRate[i][j].x = currentOrthoRates[0];
            orthoSpreadRate[i][j].y = currentOrthoRates[1];
            orthoSpreadRate[i][j].z = currentOrthoRates[2];
            orthoSpreadRate[i][j].w = currentOrthoRates[3];

            timeStamp = currentStamp;
            sourceDataTexture[i][j].x = newRate;

      }
   }
}

/*
Function: burnDistance
Input: TBD
Shader base: partialBurn
Purpose: 
*/
void fireSim::burnDistance(){

}

/*
Function: accelerateFire
Input: TBD
Shader base: acceleration
Purpose: 
*/
void fireSim::accelerateFire()   {

}

/*
Function: triggerNextEvent
Input: TBD
Purpose: Step time through simulation
*/
void fireSim::triggerNextEvent(){

}


/*//////////////////////////////////////////////////////////////////
                     Support Functions
//////////////////////////////////////////////////////////////////*/


/*
Function: accelerate
Input: TBD
Purpose: run acceleration code
*/
float* fireSim::accelerate(float* current, float* maxRate, float dt){
   for(int i = 0; i < 4; i++){
      current[i] = min(current[i], maxRate[i]);
   }
   // clamp
   float ratio[4];
   for(int i = 0; i < 4; i++){
      float tmp = current[i] / maxRate[i];
      if(tmp < 0.1){
         ratio[i] = 0.1;
      }
      else if(tmp > 0.9){
         ratio[i] = 0.9;
      }
      else
         ratio[i] = tmp;
   }

   // find timeToMax
   float timeToMax[4];
   for(int i = 0; i < 4; i++){
      timeToMax[i] = -log(1.0 - ratio[i]) / accelerationConstant;
   }

   // clamp
   for(int i = 0; i < 4; i++){
      float tmp = dt / timeToMax[i];
      if(tmp < 0.0){
         tmp = 0.0;
      }
      else if(tmp > 1.0){
         tmp = 1.0;
      }

      current[i] = tmp * (maxRate[i] - current[i]) + current[i];
   }

   return current;
}

/*
Function: testCrownRate
Input: TBD
Purpose: tests crown rate in each update step
*/
float fireSim::testCrownRate(float spreadRate,
               float maxRate,
                    float intensityModifier, 
               float crownActiveRate, 
               float crownThreshold)
{
   if(maxRate <= 0.0)
      return 0.0;

   spreadRate *= 60.0;
   spreadRate /= 3.28;
   maxRate *= 60.0;
   maxRate /= 3.28;
   float firelineIntensity = spreadRate * intensityModifier;
   if(firelineIntensity > crownThreshold){
      float maxCrownRate = 3.34 * maxRate;
      float surfaceFuelConsumption = crownThreshold * spreadRate / firelineIntensity;
      float crownCoefficient = -log(0.1)/(0.9 * (crownActiveRate - surfaceFuelConsumption));
      float crownFractionBurned = 1.0 - exp(-crownCoefficient * (spreadRate - surfaceFuelConsumption));
      if(crownFractionBurned < 0.0)
         crownFractionBurned = 0.0;
      if(crownFractionBurned > 1.0)
         crownFractionBurned = 1.0;
      float crownRate = spreadRate + crownFractionBurned * (maxCrownRate - spreadRate);
      if(crownRate >= crownActiveRate)
         maxRate = max(crownRate, maxRate);

   }
   return maxRate * 3.28 / 60.0;
}