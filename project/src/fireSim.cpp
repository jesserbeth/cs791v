#include "fireSim.h"
#include <algorithm>
#include <math.h>

/* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW */
static int nCol[8] =        {  0,  1,  1,  1,  0, -1, -1, -1};
static int nRow[8] =        {  1,  1,  0, -1, -1, -1,  0,  1};

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
   rothData = new vec4*[simDimX];
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

   // rothermel vals
   fuelTexture = new float*[simDimX];
   deadSAVBurnableBuffer = new vec4*[simDimX];
   dead1hBuffer = new vec4*[simDimX];
   dead10hBuffer = new vec4*[simDimX];
   dead100hBuffer = new vec4*[simDimX];
   liveHBuffer = new vec4*[simDimX];
   liveWBuffer = new vec4*[simDimX];
   fineDeadExctinctionsDensityBuffer = new vec4*[simDimX];
   areasReactionFactorsBuffer = new vec4*[simDimX];
   slopeWindFactorsBuffer = new vec4*[simDimX];
   residenceFluxLiveSAVBuffer = new vec4*[simDimX];
   fuelSAVAccelBuffer = new vec2*[simDimX];
   slopeAspectElevationBuffer = new vec3*[simDimX];
   windTexture = new vec2*[simDimX];
   deadMoisturesTexture = new vec3*[simDimX];
   liveMoisturesTexture = new vec2*[simDimX];


   for(int i = 0; i < simDimX; i++){
      timeOfArrival[i] = new float[simDimY];
      rothData[i] = new vec4[simDimY];
      deadSAVBurnableBuffer[i] = new vec4[simDimY];
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

      // rothermel
      fuelTexture[i] = new float[simDimY];
      dead1hBuffer[i] = new vec4[simDimY];
      dead10hBuffer[i] = new vec4[simDimY];
      dead100hBuffer[i] = new vec4[simDimY];
      liveHBuffer[i] = new vec4[simDimY];
      liveWBuffer[i] = new vec4[simDimY];
      fineDeadExctinctionsDensityBuffer[i] = new vec4[simDimY];
      areasReactionFactorsBuffer[i] = new vec4[simDimY];
      slopeWindFactorsBuffer[i] = new vec4[simDimY];
      residenceFluxLiveSAVBuffer[i] = new vec4[simDimY];
      fuelSAVAccelBuffer[i] = new vec2[simDimY];
      slopeAspectElevationBuffer[i] = new vec3[simDimY];
      windTexture[i] = new vec2[simDimY];
      deadMoisturesTexture[i] = new vec3[simDimY];
      liveMoisturesTexture[i] = new vec2[simDimY];
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
         rothData[i][j].x = rothData[i][j].y = rothData[i][j].z = 0.;
         fuelTexture[i][j] = 151.;
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

         // Rothermel Data Members
         fuelTexture[i][j] = 100.;
         deadSAVBurnableBuffer[i][j].x = deadSAVBurnableBuffer[i][j].y = deadSAVBurnableBuffer[i][j].z = deadSAVBurnableBuffer[i][j].w = 200.;
   // cout << ";klajdfjkl;" << endl;
         dead1hBuffer[i][j].x = dead1hBuffer[i][j].y = dead1hBuffer[i][j].z = dead1hBuffer[i][j].z = 1.;
         dead10hBuffer[i][j].x = dead10hBuffer[i][j].y = dead10hBuffer[i][j].z = dead10hBuffer[i][j].w = 1.;
         dead100hBuffer[i][j].x = dead100hBuffer[i][j].y = dead100hBuffer[i][j].z = dead100hBuffer[i][j].w = 1.;
         liveHBuffer[i][j].x = liveHBuffer[i][j].y = liveHBuffer[i][j].z = liveHBuffer[i][j].w = 1.;
         liveWBuffer[i][j].x = liveWBuffer[i][j].y = liveWBuffer[i][j].z = liveWBuffer[i][j].w = 1.;
         fineDeadExctinctionsDensityBuffer[i][j].x = fineDeadExctinctionsDensityBuffer[i][j].y = fineDeadExctinctionsDensityBuffer[i][j].z = fineDeadExctinctionsDensityBuffer[i][j].w = 1.;
         areasReactionFactorsBuffer[i][j].x = areasReactionFactorsBuffer[i][j].y = areasReactionFactorsBuffer[i][j].z = areasReactionFactorsBuffer[i][j].w = 1.;
         slopeWindFactorsBuffer[i][j].x = slopeWindFactorsBuffer[i][j].y = slopeWindFactorsBuffer[i][j].z = slopeWindFactorsBuffer[i][j].w = 1.;
         residenceFluxLiveSAVBuffer[i][j].x = residenceFluxLiveSAVBuffer[i][j].y = residenceFluxLiveSAVBuffer[i][j].z = residenceFluxLiveSAVBuffer[i][j].w = 1.;
         fuelSAVAccelBuffer[i][j].x = fuelSAVAccelBuffer[i][j].y = 1.;
         slopeAspectElevationBuffer[i][j].x = slopeAspectElevationBuffer[i][j].y = slopeAspectElevationBuffer[i][j].z = 1.;
         windTexture[i][j].x = windTexture[i][j].y = 1.;
         deadMoisturesTexture[i][j].x = deadMoisturesTexture[i][j].y = deadMoisturesTexture[i][j].z = 1.;
         liveMoisturesTexture[i][j].x = liveMoisturesTexture[i][j].y = 1.;
         // cout << "test" << endl;
      }
   }
   spreadData[5][5] = 100;
   // cout << "HERE" << endl;

}

/*
   NOTES FOR HOW I GOT THE DATA D: (PLEASE LET THIS BE RIGHT)
      Data I need                   Place I get it          (how it is produced)
      deadXh, liveX:                effectiveHeatingNumber  (calculated in fuelmodel)
                                    load                    (from .fmd)
                                    areaWeightingFactor     (calculated in fuelmodel)
                                    FuelMoisture            (all zeros)
      
      fineDeadExctinctionDensity:   fineDeadRatio           (calculated in fuelmodel)
                                    extinctionMoisture      (from .fmd)
                                    liveExtinction          (calculated in fuelmodel)
                                    fuelDensity             (calculated in fuelmodel)

      areasReactionFactors:         deadArea                (calculated in fuelmodel)
                                    liveArea                (calculated in fuelmodel)
                                    deadReactionFactor      (calculated in fuelmodel)
                                    liveReactionFactor      (calculated in fuelmodel)

      slopeWindFactors:             slopeK                  (calculated in fuelmodel)
                                    windK                   (calculated in fuelmodel)
                                    windB                   (calculated in fuelmodel)
                                    windE                   (calculated in fuelmodel)

      residenceFluxLiveSAV:         residenceTime           (calculated in fuelmodel)
                                    propagatingFlux         (calculated in fuelmodel)
                                    SAV                     (from .fmd)
                                    SAV                     (from .fmd)

      deadSAVBurnable:              SAV                     (from .fmd)
                                    SAV                     (from .fmd)
                                    SAV                     (from .fmd)
                                    burnable                (always true)           

      fuelSAVAccel:                 fuelSAV                 (calculated in fuelmodel)
                                    accelerationConstant    (0.115)
*/

/*
Function: updateSpreadData
Input: The necessary inputs are the values that are found in the textures/buffers
       in the FuelModel.h/Simulator.cpp files in Roger's code
Shader base: rothermel
Purpose: This runs rothermel's equations to initialize simulation
*/
void fireSim::updateSpreadData(){
   // This is where rothermel's shader is implemented
   int    nrow, ncol, ncell;   /* row, col, and index of neighbor cell */
   float dist = 10.;
   
   for(int i = 0; i < simDimX; i++){
      for(int j = 0; j < simDimY; j++){
         // Shader code: int fuelModel = texture2D(fuelTexture, gl_TexCoord[1].st).r;
            // gl_TexCoord[1].st corresponds to fuelTexture.xy
         int fuelModel = fuelTexture[i][j];
         vec4 dead1h, deadSAVBurnable, dead10h, dead100h, liveH, 
              liveW, fineDeadExtinctionsDensity, areasReactionFactors,
              slopeWindFactors, residenceFluxLiveSAV;
         vec2 fuelSAVAccel;

         vec3 slopeAspectElevation;
         vec2 wind;
         vec3 deadMoistures;
         vec2 liveMoistures;

         float maxSpreadRate = 0.0;
         float ellipseEccentricity = 0.0;
         float spreadDirection = 0.0;
         float spreadModifier = 0.0;
         vec3 timeLagClass;

         // Get data into vars
         deadSAVBurnable = deadSAVBurnableBuffer[i][j];
         if(deadSAVBurnable.w < 50.0){
            continue;
         }

         dead1h = dead1hBuffer[i][j];
         dead10h = dead10hBuffer[i][j];
         dead100h = dead100hBuffer[i][j];
         liveH = liveHBuffer[i][j];
         liveW = liveWBuffer[i][j];
         fineDeadExtinctionsDensity = fineDeadExctinctionsDensityBuffer[i][j];
         areasReactionFactors = areasReactionFactorsBuffer[i][j];
         slopeWindFactors = slopeWindFactorsBuffer[i][j];
         residenceFluxLiveSAV = residenceFluxLiveSAVBuffer[i][j];
         fuelSAVAccel = fuelSAVAccelBuffer[i][j];
         float fuelSAV = fuelSAVAccel.x;
         float accelerationConstant = fuelSAVAccel.y;

         slopeAspectElevation = slopeAspectElevationBuffer[i][j];
         wind = windTexture[i][j];
         deadMoistures = deadMoisturesTexture[i][j];
         liveMoistures = liveMoisturesTexture[i][j];


         if (deadSAVBurnable.x > 192.0)
            timeLagClass.x = deadMoistures.x;
         else if (deadSAVBurnable.x > 48.0)
            timeLagClass.x = deadMoistures.y;
         else
            timeLagClass.x = deadMoistures.z;

         if (deadSAVBurnable.y > 192.0)
            timeLagClass.y = deadMoistures.x;
         else if (deadSAVBurnable.y > 48.0)
            timeLagClass.y = deadMoistures.y;
         else
            timeLagClass.y = deadMoistures.z;

         if (deadSAVBurnable.z > 192.0)
            timeLagClass.z = deadMoistures.x;
         else if (deadSAVBurnable.z > 48.0)
            timeLagClass.z = deadMoistures.y;
         else
            timeLagClass.z = deadMoistures.z;

         float weightedFuelModel = 
            timeLagClass.x * dead1h.x * dead1h.y +
            timeLagClass.y * dead10h.x * dead10h.y +
            timeLagClass.z * dead100h.x * dead100h.y;

         float fuelMoistures[5];
         fuelMoistures[0] = timeLagClass.x;
         fuelMoistures[1] = timeLagClass.y;
         fuelMoistures[2] = timeLagClass.z;
         fuelMoistures[3] = liveMoistures.x;
         fuelMoistures[4] = liveMoistures.y;

         float liveExtinction = 0.0;
         if(liveH.y > 0.0 || liveW.y > 0.0){
            float fineDeadMoisture = 0.0;
            if (fineDeadExtinctionsDensity.x > 0.0)
               fineDeadMoisture = weightedFuelModel / fineDeadExtinctionsDensity.x;

            liveExtinction =
               (fineDeadExtinctionsDensity.z * 
                (1.0 - fineDeadMoisture / fineDeadExtinctionsDensity.y)) - 0.226;
            liveExtinction = max(liveExtinction, fineDeadExtinctionsDensity.y);
         }
         
         float heatOfIgnition =
            areasReactionFactors.x *
               ((250.0 + 1116.0 * fuelMoistures[0]) * dead1h.z * dead1h.x +
                (250.0 + 1116.0 * fuelMoistures[1]) * dead10h.z * dead10h.x +
                (250.0 + 1116.0 * fuelMoistures[2]) * dead100h.z * dead100h.x) +
            areasReactionFactors.y *
               ((250.0 + 1116.0 * fuelMoistures[3]) * liveH.z * liveH.x +
                (250.0 + 1116.0 * fuelMoistures[4]) * liveW.z * liveW.x);
         heatOfIgnition *= fineDeadExtinctionsDensity.w;

         float liveMoisture = liveH.z * fuelMoistures[3] + liveW.z * fuelMoistures[4];
         float deadMoisture = dead1h.z * fuelMoistures[0] + 
                              dead10h.z * fuelMoistures[1] + 
                              dead100h.z * fuelMoistures[2];

         float reactionIntensity = 0.0;

         if (liveExtinction > 0.0)
            {
               float r = liveMoisture / liveExtinction;
               if (r < 1.0)
                  reactionIntensity += areasReactionFactors.w * (1.0 - 
                                                                 (2.59 * r) + 
                                                                 (5.11 * r * r) - 
                                                      (3.52 * r * r * r));
            }
            if (fineDeadExtinctionsDensity.y > 0.0)
            {
               float r = deadMoisture / fineDeadExtinctionsDensity.y;
               if (r < 1.0)
                  reactionIntensity += areasReactionFactors.z * (1.0 - 
                                                                 (2.59 * r) + 
                                                                 (5.11 * r * r) - 
                                                      (3.52 * r * r * r));
            }

            float heatPerUnitArea = reactionIntensity * residenceFluxLiveSAV.x;
            float baseSpreadRate = 0.0;
            if (heatOfIgnition > 0.0)
               baseSpreadRate = reactionIntensity * residenceFluxLiveSAV.y / heatOfIgnition;
            
            float slopeFactor = slopeWindFactors.x * slopeAspectElevation.x * slopeAspectElevation.x;
            float windFactor = 0.0;
            if (wind.x > 0.0)
               windFactor = slopeWindFactors.y * pow(wind.x, slopeWindFactors.z);

            spreadModifier = slopeFactor + windFactor;
            
            float upslope;
            if (slopeAspectElevation.y >= 180.0)
               upslope = slopeAspectElevation.y - 180.0;
            else
               upslope = slopeAspectElevation.y + 180.0;

            int checkEffectiveWindspeed = 0;
            int updateEffectiveWindspeed = 0;
            float effectiveWindspeed = 0.0;
            if (baseSpreadRate <= 0.0)
            {
               maxSpreadRate = 0.0;
               spreadDirection = 0.0;
            }
            else if (spreadModifier <= 0.0)
            {
               maxSpreadRate = baseSpreadRate;
               spreadDirection = 0.0;
            }
            else if (slopeAspectElevation.x <= 0.0)
            {
               effectiveWindspeed = wind.x;
               maxSpreadRate = baseSpreadRate * (1.0 + spreadModifier);
               spreadDirection = wind.y;
               checkEffectiveWindspeed = 1;
            }
            else if (wind.x <= 0.0)
            {
               maxSpreadRate = baseSpreadRate * (1.0 + spreadModifier);
               spreadDirection = upslope;
               updateEffectiveWindspeed = 1;
               checkEffectiveWindspeed = 1;
            }
            else if (fabs(wind.y - upslope) < 0.000001)
            {
               maxSpreadRate = baseSpreadRate * (1.0 + spreadModifier);
               spreadDirection = upslope;
               updateEffectiveWindspeed = 1;
               checkEffectiveWindspeed = 1;
            }
            else
            {
               float angleDelta;
               if (upslope <= wind.y)
                  angleDelta = wind.y - upslope;
               else
                  angleDelta = 360.0 - upslope + wind.y;
               angleDelta *= 3.14159 / 180.0;
               float slopeRate = baseSpreadRate * slopeFactor;
               float windRate = baseSpreadRate * windFactor;
               float x = slopeRate + windRate * cos(angleDelta);
               float y = windRate * sin(angleDelta);
               float addedSpeed = sqrt(x * x + y * y);
               maxSpreadRate = baseSpreadRate + addedSpeed;

               spreadModifier = maxSpreadRate / baseSpreadRate - 1.0;
               if (spreadModifier > 0.0)
                  updateEffectiveWindspeed = 1;
               checkEffectiveWindspeed = 1;

               float addedAngle = 0.0;
               if (addedSpeed > 0.0)
                  addedAngle = asin(clamp(fabs(y) / addedSpeed, -1.0, 1.0));
               float angleOffset = 0.0;
               if (x >= 0.0)
               {
                  if (y >= 0.0)
                     angleOffset = addedAngle;
                  else
                     angleOffset = 2.0 * 3.14159 - addedAngle;
               }
               else
               {
                  if (y >= 0.0)
                     angleOffset = 3.14159 + addedAngle;
                  else
                     angleOffset = 3.14159 - angleOffset;
               }
               spreadDirection = upslope + angleOffset * 180.0 / 3.14159;
               if (spreadDirection > 360.0)
                  spreadDirection -= 360.0;
            }

            if (updateEffectiveWindspeed == 1)
            {
               effectiveWindspeed = pow((spreadModifier * slopeWindFactors.w), (1.0 / slopeWindFactors.z));
            }
            if (checkEffectiveWindspeed == 1)
            {
               float maxWind = 0.9 * reactionIntensity;
               if (effectiveWindspeed > maxWind)
               {
                  if (maxWind <= 0.0)
                     spreadModifier = 0.0;
                  else
                     spreadModifier = slopeWindFactors.y * pow(maxWind, slopeWindFactors.z);
                  maxSpreadRate = baseSpreadRate * (1.0 + spreadModifier);
                  effectiveWindspeed = maxWind;
               }
            }
            ellipseEccentricity = 0.0;
            if (effectiveWindspeed > 0.0)
            {
               float lengthWidthRatio = 1.0 + 0.002840909 * effectiveWindspeed;
               ellipseEccentricity = sqrt(lengthWidthRatio * lengthWidthRatio - 1.0) / lengthWidthRatio;
            }
            //maxSpreadRate = maxSpreadRate * (1.0 - exp(-accelerationConstant * burnTime / 60.0));
            //float firelineIntensity = 
            // 3.4613 * (384.0 * (reactionIntensity / 0.189275)) * 
            //    (maxSpreadRate * 0.30480060960) / (60.0 * fuelSAV);
            //firelineIntensity =
            // reactionIntensity * 12.6 * maxSpreadRate / (60.0 * fuelSAV);
            float intensityModifier =
               3.4613 * (384.0 * (reactionIntensity / 0.189275)) *
                  (0.30480060960) / (60.0 * fuelSAV);
            // gl_FragData[0] = vec4(maxSpreadRate, 
            //                       ellipseEccentricity, spreadDirection, intensityModifier);

            rothData[i][j].x = maxSpreadRate;
            cout << maxSpreadRate;
            rothData[i][j].y = spreadDirection;
            cout << spreadDirection;
            rothData[i][j].z = ellipseEccentricity;
            cout << ellipseEccentricity << endl;

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

/*
Function: testCrownRate
Input: TBD
Purpose: tests crown rate in each update step
*/
float fireSim::clamp(float val, float flr, float ceiling){
   if(val >= flr && val <= ceiling){
      return val;
   }
   if(val < flr){
      return flr;
   }
   return ceiling;
}