#include <iostream>
#include <vector>

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

class vec4{
   public: 
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

      float* accelerate(float*, float*, float);
      float testCrownRate(float,float,float,float,float);

   // private: 

      // Simulation Data
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

      int simDimX;
      int simDimY;
};

