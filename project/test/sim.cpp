#include <iostream>
#include <fstream>
#include <math.h>

// using namespace std;

const int INF = 2000000;

float burnDistance(float dist, float rate, float timeStep){
    // lower distance based on roth rate
        // t = d / r;
        // d = d - r * timeStep
    dist = dist - rate * timeStep;
    if( dist < 0){
        dist = 0;
    }
    return dist;
}


int main(){
    // vars
    std::ofstream fout;
    fout.open("test.csv");
    // int totalTime = 100;
    float timeStep = .5;
    int Rows = 100;
    int Cols = 100;
    int CELLS = Rows * Cols;
    // int ignMap[CELLS];
    // int ignMapStep[CELLS];
    // float rateMap[CELLS];
    // std::cout << "temp" << std::endl;
    float *ignTime = new float[CELLS];
    float *ignTimeNew = new float[CELLS];
    // float rateMap[Rows*Cols];
    // float baseDistance = 5.;
    // float** burnDist = new float*[Rows*Cols];
    // for(int i = 0; i < Rows*Cols; i++){
    //     // burnDist[i] = new float[8];
    //     burnDist[i] = new float[12];
    // }
    for(int i = 0; i < Rows*Cols; i++){
        // ignMap[i] = 0;
        // ignMapStep[i] = 0;
        ignTime[i] = INF;
        ignTimeNew[i] = INF;
        // rateMap[i] = 0.0;
        // for(int j = 0; j < 8; j++){
        //     if(j % 2 == 1){
        //         burnDist[i][j] = baseDistance * sqrt(2);
        //         // std::cout << burnDist[i][j] << std::endl;
        //     }
        //     else
        //         burnDist[i][j] = baseDistance;
        // }
        // for(int j = 8; j < 12; j++){
        //     burnDist[i][j] = 2 * baseDistance * sqrt(2);
        // }
    }
    // std::cout << "blah" << std::endl;
    // for(int i = 0; i < Rows*Cols; i++){
    //     for(int j = 0; j < 8; j++){
    //         std::cout << burnDist[i][j] << ' ';
    //     }
    //     if(i % Rows == 0)
    //         std::cout << std::endl;
    // }
    int row, col, cell, ncell, ncol, nrow;
    float cellSize = 10;
    float orthoSize = cellSize;
    float diagSize = cellSize * sqrt(2);
    float superSize = sqrt(pow(cellSize, 2) + pow(cellSize*2, 2));
    /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW */
    // static int nCol[8] =        {  0,  1,  1,  1,  0, -1, -1, -1};
    // static int nRow[8] =        {  1,  1,  0, -1, -1, -1,  0,  1};
    // static float angle[8]  =    {  90, 45, 0, 45, 90, 45, 0, 45};
    /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW  NNW NNE NEE SEE SSE SSW SWW NWW*/
    static int nCol[16] =        {  0,  1,  1,  1,  0, -1, -1, -1, -1, 1, 2, 2, 1, -1, -2, -2};
    static int nRow[16] =        {  1,  1,  0, -1, -1, -1,  0,  1, 2, 2, 1, -1, -2, -2, -1, 1};
    static float angle[16] =     {   90, 45, 0, 45, 90, 45, 0, 45, 
                                     26.57, 63.43, 26.57, 26.57, 63.43, 63.43, 26.57, 26.57};
    static float L_n[16] =        { orthoSize, diagSize, orthoSize, diagSize, orthoSize, diagSize,
                                    orthoSize, diagSize, superSize,superSize,superSize,superSize,
                                    superSize,superSize,superSize,superSize};
    //ignite point
    // ignMap[5250] = 1;
    // ignMap[3400] = 1;
    // rateMap[5250] = .4;
    // rateMap[3400] = 3.;
    int ignSpot = Rows * Cols / 2 + Cols / 2;
    ignTime[ignSpot] = 0;
    ignTimeNew[ignSpot] = 0;
    ignSpot /= 2;
    ignTime[ignSpot] = 0;
    ignTimeNew[ignSpot] = 0;
    // ignTime[3400] = 0;
    // bool spreadFire = false;
    // int stepCounter = 0;

    // loop through time:
    // for(int t = 0; t < 2000; t++){
    //     for ( cell=0, row=0; row<Rows; row++ ){
    //         for ( col=0; col<Cols; col++, cell++ ){
    //             // check if already "ignited"
    //             if(ignMap[cell] == 0){
    //                 continue;
    //             }
    //             // std::cout << row << ' ' << ignMap[cell] << std::endl;
    //             // check neighbors for ignition
    //             spreadFire = true;
    //             for(int n = 0; n < 8; n++){
    //                 nrow = row + nRow[n];
    //                 ncol = col + nCol[n];
    //                 if ( nrow<0 || nrow>=Rows || ncol<0 || ncol>=Cols )
    //                     continue;
    //                 ncell = ncol + nrow*Cols;

    //                 // check for already lit
    //                 if(ignMap[ncell] == 1){
    //                     continue;
    //                 }

    //                 // Burn distance 
    //                 // burnDist[ncell] -= 1;
    //                 // propogateFire(startTime, stepTime);
    //                 burnDist[ncell][n] = burnDistance(burnDist[ncell][n], rateMap[cell], timeStep);
                    
    //                 // std::cout << burnDist[ncell][n]<< std::endl;
    //                 // Propogate fire step:
    //                 if(burnDist[ncell][n] == 0){
    //                     ignMapStep[ncell] = 1;
    //                     ignTime[ncell] = t;
    //                     rateMap[ncell] = rateMap[cell];
                        
    //                 }
    //             }
    //         }
    //         // stepCounter++;
    //     }
    //     for(int i = 0; i < Rows*Cols; i++){
    //         if(ignMapStep[i] == 1)
    //             ignMap[i] = 1;
    //     }
    // }
    float timeNext = 0.;
    float timeNow = 0.;
    float ROS = .4 * (1.0 - .2)/(1 - .2 * cos(30));
    // float L_n = 3.0;
    // int cell = 0;
    // int ncell, nrow, ncol;
    int counter = 0;


    // while(timeNext < INF && counter < 200000){
    //     timeNow = timeNext;
    //     timeNext = INF;
    //     counter++;

    //     // Loop through all cells
    //     // for(cell = 0; cell < CELLS; cell++){
    //     for ( cell=0, row=0; row<Rows; row++ ){
    //         for ( col=0; col<Cols; col++, cell++ ){
    //             if(timeNext > ignTime[cell] && ignTime[cell] > timeNow){
    //                 timeNext = ignTime[cell];
    //             }
    //             else if( ignTime[cell] == timeNow){
    //                 // for(int n = 0; n < 16; n++){
    //                 for(int n = 0; n < 8; n++){
    //                     // find neighbor cell index
    //                     nrow = row + nRow[n];
    //                     ncol = col + nCol[n];
    //                     // std::cout << row << ' ' << col << ' ' << std::endl;
    //                     if ( nrow<0 || nrow>= Rows || ncol<0 || ncol>=  Cols )
    //                         continue;
    //                     ncell = ncol + nrow*Cols;

    //                     // If neighbor is unburned
    //                     if(ignTime[ncell] > timeNow){
    //                         // compute ignition time
    //                         // ROS = blah blah
    //                         ROS = .4 * (1.0 - .6)/(1 - .6 * cos(angle[n]));

    //                         float ignTimeNew = timeNow + L_n[n] / ROS;

    //                         if(ignTimeNew < ignTime[ncell]){
    //                             ignTime[ncell] = ignTimeNew;
    //                         }
    //                         if(ignTimeNew < timeNext){
    //                             timeNext = ignTimeNew;
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }


    ///////////////////////////////////////////////////////////////////////////////////////////
    //                              Iterative Minimal Time
    ///////////////////////////////////////////////////////////////////////////////////////////
    float ignCell = 0.;
    float ignTimeMin = INF;
    while(counter < 2000){
        counter++;

        // Loop through all cells
        // for(cell = 0; cell < CELLS; cell++){
        for ( cell=0, row=0; row<Rows; row++ ){
            for ( col=0; col<Cols; col++, cell++ ){
                ignCell = ignTime[cell];

                if(ignCell > 0){
                    ignTimeMin = INF;
                    // Loop through neighbors
                    for(int n = 0; n < 16; n++){
                        // find neighbor cell index
                        nrow = row + nRow[n];
                        ncol = col + nCol[n];
                        // std::cout << row << ' ' << col << ' ' << std::endl;
                        if ( nrow<0 || nrow>= Rows || ncol<0 || ncol>=  Cols )
                            continue;
                        ncell = ncol + nrow*Cols;

                        ROS = .4 * (1.0 - .6)/(1 - .6 * cos(angle[n]));
                        float ignTimeNew = ignTime[ncell] + L_n[n] / ROS;
                        ignTimeMin = ignTimeNew*(ignTimeNew < ignTimeMin) + ignTimeMin*(ignTimeNew >= ignTimeMin);
                        // if(ignTimeNew < ignTimeMin)
                        //     ignTimeNew = ignTimeNew;
                        // else if(ignTimeNew >= ignTimeMin)
                        //     ignTimeMin = ignTimeMin;
                        // std::cout << ignTimeNew << ' ';
                    }
                    ignTimeNew[cell] = ignTimeMin;
                }
            }
            // std::cout << std::endl;
        }

        // Swap pointers to loop
        float *temp = ignTime;
        ignTime = ignTimeNew;
        ignTimeNew = temp;
        // std::cout << counter << std::endl;
    }


    for(int i = 0; i < Rows*Cols; i++){
        // std::cout << ignTime[i] << " ,";
        if(i %Rows == 0 && i !=0){
            // std::cout << std::endl;
            fout << '\n';
        }
        fout << (int)ignTime[i] << " ";
        // fout << (int)ignTimeNew[i] << " ";
    }
    fout.close();
}