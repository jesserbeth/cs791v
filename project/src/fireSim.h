#include <iostream>
#include <vector>
#include "FuelModel.h"
#include "FuelMoisture.h"


using namespace std;

class point{
   public:
      point(){
         x = 0;
         y = 0;
      };
      int x;
      int y;
};

class vec2{
   public: 
      vec2(){
         x = y = 0;
      };
      vec2(float _x, float _y){
         x = _x;
         y = _y;
      };
      vec2 operator=(const vec2& vector){
         float _x = vector.x;
         float _y = vector.y;
         return vec2(_x,_y);
      };
      float x,y;
};
class vec3{
   public: 
      vec3(){
         x = y = z = 0;
      };
      vec3(float _x, float _y, float _z){
         x = _x;
         y = _y;
         z = _z;
      };
      vec3 operator=(const vec3& vector){
         float _x = vector.x;
         float _y = vector.y;
         float _z = vector.z;
         return vec3(_x,_y,_z);
      };
      float x,y,z;
};
class vec4{
   public: 
      vec4(){
         x = y = z = w = 0;
      };
      vec4(float _x, float _y, float _z, float _w){
         x = _x;
         y = _y;
         z = _z;
         w = _w;
      };
      vec4 operator=(const vec4& vector){
         float _x = vector.x;
         float _y = vector.y;
         float _z = vector.z;
         float _w = vector.w;
         return vec4(_x,_y,_z,_w);
      };
      float x,y,z,w;
};

class fireSim{
   public: 
      fireSim(); // this will set to default test state
      ~fireSim();
      void init();
      void updateSpreadData();
      void propagateFire();
      void burnDistance();
      void accelerateFire();
      void triggerNextEvent();
      float clamp(float, float, float);

      float* accelerate(float*, float*, float);
      float testCrownRate(float,float,float,float,float);

   // private: 

      // Simulation Data
      vec4** rothData; // x - maxSpreadRate, y - spreadDirection, z - ellipseEccentricity
      float** timeOfArrival;
      float** originalTimeOfArrival;
      vec4** orthoSpreadRate;
      vec4** diagSpreadRate;
      vec4** orthoMaxSpreadRate;
      vec4** diagMaxSpreadRate;
      vec4** orthoBurnDistance;
      vec4** diagBurnDistance;
      int**   updateStamp;
      point** sourceDataTexture;

      float** crownThreshold;
      float** crownActiveRate;
      float** canopyHeight;
      float** spreadData;

      float startTime; 
      float baseTime;
      float endTime;
      int lastStamp;
      int currentStamp;
      float accelerationConstant;
      
      float outputTOA;
      vec4* outputOrthoRates;
      vec4* outputDiagRates;
      int timeStamp;
      float* outputSourceData;

      // Rothermel Data Members
      float** fuelTexture;
      vec4*  deadSAVBurnableBuffer;
      vec4*  dead1hBuffer;
      vec4*  dead10hBuffer;
      vec4*  dead100hBuffer;
      vec4*  liveHBuffer;
      vec4*  liveWBuffer;
      vec4*  fineDeadExtinctionsDensityBuffer;
      vec4*  areasReactionFactorsBuffer;
      vec4*  slopeWindFactorsBuffer;
      vec4*  residenceFluxLiveSAVBuffer;
      vec2*  fuelSAVAccelBuffer;
      // Roth textures
      vec3** slopeAspectElevationTexture;
      vec2** windTexture;
      vec3** deadMoisturesTexture;
      vec2** liveMoisturesTexture;

      int simDimX;
      int simDimY;

      std::vector<sim::FuelModel> _models;
      std::vector<sim::FuelMoisture> _moistures;
};

