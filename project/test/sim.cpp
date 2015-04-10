#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>

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
    // int totalTime = 100;
    // for(int T = 1024; T < 1025; T<<=1){
    for(int T = 256; T < 257; T<<=1){
        std::cout << "T: " << T << std::endl;
    // fout.open("test.csv");
    struct timeval start, fin;

    // ROTHERMEL CONSTANTS
    float maxSpreadRate = 10.8522;
    float spreadDirection = 10;
    float ellipseEccentricity = 0.233421;



    float timeStep = .5;
    // int Rows = 10000;
    // int Cols = 10000;
    int Rows = T;
    int Cols = T;
    int CELLS = Rows * Cols;
    int *ignMap = new int[CELLS];
    int *ignMapStep = new int[CELLS];
    float *rateMap = new float[CELLS];
    // std::cout << "temp" << std::endl;
    float *ignTime = new float[CELLS];
    float *ignTimeNew = new float[CELLS];
    // float rateMap[Rows*Cols];
    float baseDistance = 5.;
    float** burnDist = new float*[Rows*Cols];
    for(int i = 0; i < Rows*Cols; i++){
        // burnDist[i] = new float[8];
        burnDist[i] = new float[12];
    }
    for(int i = 0; i < Rows*Cols; i++){
        ignMap[i] = 0;
        ignMapStep[i] = 0;
        ignTime[i] = 0;
        ignTime[i] = INF;
        ignTimeNew[i] = INF;
        rateMap[i] = 0.0;
        for(int j = 0; j < 8; j++){
            if(j % 2 == 1){
                burnDist[i][j] = baseDistance * sqrt(2);
                // std::cout << burnDist[i][j] << std::endl;
            }
            else
                burnDist[i][j] = baseDistance;
        }
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
    float cellSize = 30;
    float orthoSize = cellSize;
    float diagSize = cellSize * sqrt(2);
    float superSize = sqrt(pow(cellSize, 2) + pow(cellSize*2, 2));
    float pi = 3.14159;
    /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW */
    // static int nCol[8] =        {  0,  1,  1,  1,  0, -1, -1, -1};
    // static int nRow[8] =        {  1,  1,  0, -1, -1, -1,  0,  1};
    // static float angle[8]  =    {  90, 45, 0, 45, 90, 45, 0, 45};
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
    //ignite point
    // rateMap[3400] = 3.;
    int ignSpot = Rows * Cols / 2 + Cols / 2;
    ignMap[ignSpot] = 1;
    // ignMap[3400] = 1;
    rateMap[ignSpot] = 4.;
    ignTime[ignSpot] = 0;
    // ignTime[ignSpot] = 0;
    // ignTimeNew[ignSpot] = 0;
    // ignSpot /= 2;
    // ignTime[ignSpot] = 0;
    // ignTimeNew[ignSpot] = 0;
    // ignTime[3400] = 0;
    // bool spreadFire = false;
    // int stepCounter = 0;
    // int edgeCount = 0;
    // int edgeNum = Rows*4;
    int corner = 0;

    // char threadNum[21];
    // sprintf(threadNum, "_%d", T);
    // char f_name[] = "data_Roger_";
    // char csv[] = ".csv";
    // strcat(f_name,threadNum);
    // strcat(f_name,csv);
    // fout.open(f_name);


    // // loop through time:
    // // BURNING DISTANCES
    // gettimeofday(&start, NULL);
    // // for(int t = 0; t < 20000; t++){
    // for(int t = 1; t < 20000; t++){
    //     for ( cell=0, row=0; row<Rows; row++ ){
    //         for ( col=0; col<Cols; col++, cell++ ){
    //             // check if already "ignited"
    //             if(ignMap[cell] == 0){
    //                 continue;
    //             }
    //             // std::cout << row << ' ' << ignMap[cell] << std::endl;
    //             // check neighbors for ignition
    //             // spreadFire = true;
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
    //                     if(nrow == (Rows-1) && ncol == (Cols-1)){
    //                         corner += 1;
    //                     }
    //                     if(nrow == 0 && ncol == (Cols-1)){
    //                         corner += 1;
    //                     }
    //                     if(nrow == 0 && ncol == 0){
    //                         corner += 1;
    //                     }
    //                     if(nrow == (Rows-1) && ncol == 0){
    //                         corner += 1;
    //                     }

                        
    //                 }

    //             }
    //         }
    //         // stepCounter++;
    //     }
    //     // std::cout << edgeCount << std::endl;
    //     for(int i = 0; i < Rows*Cols; i++){
    //         if(ignMapStep[i] == 1)
    //             ignMap[i] = 1;
    //     }
    //     if(corner == 4)
    //         break;
    //     // std::cout << t << std::endl;
    // }
    // gettimeofday(&fin, NULL);
    // double t = fin.tv_usec + fin.tv_sec * 1000000.0;
    // t -= start.tv_usec + start.tv_sec * 1000000.0;
    // t /= 1000000.0;
    // std::cout << "Processing " << T << " threads took " << t << " seconds" << std::endl;




    // for(int i = 0; i < Rows*Cols; i++){
    //     // std::cout << ignTime[i] << " ,";
    //     if(i %Rows == 0 && i !=0){
    //         // std::cout << std::endl;
    //         fout << '\n';
    //     }
    //     fout << (int)ignTime[i] << " ";
    //     // fout << (int)ignTimeNew[i] << " ";
    // }
    // fout.close();



    float timeNext = 0.;
    float timeNow = 0.;
    float ROS = .4 * (1.0 - .2)/(1 - .2 * cos(30));
    // float L_n = 3.0;
    // int cell = 0;
    // int ncell, nrow, ncol;
    int counter = 0;


    char threadNum[21];
    sprintf(threadNum, "_%d", T);
    // char f_name[] = "data_MT";
    char f_name[] = "../out/test";
    char csv[] = ".csv";
    strcat(f_name,threadNum);
    strcat(f_name,csv);
    fout.open(f_name);

    // Start Timer
    gettimeofday(&start, NULL);
    // while(timeNext < INF && counter < 200000){
    while(timeNext < INF){
        timeNow = timeNext;
        timeNext = INF;
        counter++;

        // Loop through all cells
        // for(cell = 0; cell < CELLS; cell++){
        for ( cell=0, row=0; row<Rows; row++ ){
            for ( col=0; col<Cols; col++, cell++ ){
                if(timeNext > ignTime[cell] && ignTime[cell] > timeNow){
                    timeNext = ignTime[cell];
                }
                else if( ignTime[cell] == timeNow){
                    for(int n = 0; n < 16; n++){
                    // for(int n = 0; n < 8; n++){
                        // find neighbor cell index
                        nrow = row + nRow[n];
                        ncol = col + nCol[n];
                        // std::cout << row << ' ' << col << ' ' << std::endl;
                        if ( nrow<0 || nrow>= Rows || ncol<0 || ncol>=  Cols )
                            continue;
                        ncell = ncol + nrow*Cols;

                        // If neighbor is unburned
                        if(ignTime[ncell] > timeNow){
                            // compute ignition time
                            // ROS = blah blah
                            // ROS = .4 * (1.0 - .6)/(1 - .6 * cos(angle[n]));
                            ROS = maxSpreadRate * (1.0 - ellipseEccentricity) / 
                                  (1.0 - ellipseEccentricity* cos(spreadDirection * pi/180));
                            // std::cout << ROS << std::endl;
                            float ignTimeNew = timeNow + L_n[n] / ROS;

                            if(ignTimeNew < ignTime[ncell]){
                                ignTime[ncell] = ignTimeNew;
                            }
                            if(ignTimeNew < timeNext){
                                timeNext = ignTimeNew;
                            }
                        }
                    }
                }
            }
        }
        // if(counter % 1000 == 0)
            // std::cout << counter << std::endl;
    }
    // End Timer

    gettimeofday(&fin, NULL);
    double t = fin.tv_usec + fin.tv_sec * 1000000.0;
    t -= start.tv_usec + start.tv_sec * 1000000.0;
    t /= 1000000.0;
    std::cout << "Processing " << T << " threads took " << t << " seconds" << std::endl;


    // Write data to file

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


    ///////////////////////////////////////////////////////////////////////////////////////////
    //                              Iterative Minimal Time
    ///////////////////////////////////////////////////////////////////////////////////////////
    // float ignCell = 0.;
    // float ignCellNew = 0.;
    // float ignTimeMin = INF;
    // int simCounter = 0;
    

    // char threadNum[21];
    // sprintf(threadNum, "_%d", T);
    // char f_name[] = "data_IterativeMT_";
    // char csv[] = ".csv";
    // strcat(f_name,threadNum);
    // strcat(f_name,csv);
    // fout.open(f_name);
    // // int minIterations = 100;
    // bool* check = new bool[Rows*Cols];
    // for(int z = 0; z < Rows*Cols; z++){
    //     check[z] = false;
    // }

    // gettimeofday(&start, NULL);

    // // while(counter < 200){
    // while(simCounter < Rows*Cols){
    //     counter++;

    //     // Loop through all cells
    //     // for(cell = 0; cell < CELLS; cell++){
    //     for ( cell=0, row=0; row<Rows; row++ ){
    //         for ( col=0; col<Cols; col++, cell++ ){
    //             if(check[cell] == true)
    //                 continue;
    //             // Check for simulation completion
    //             ignCell = ignTime[cell];
    //             ignCellNew = ignTimeNew[cell];
    //             // std::cout << ignCell << ' ' << ignTimeNew[cell];
    //             if(abs(ignTime[cell] - ignTimeNew[cell]) < .001 && ignCell != INF
    //                     && ignCellNew != INF && check[cell] != true){
    //                 simCounter++;
    //                 check[cell] = true;
    //                 continue;
    //             }

    //             if(ignCell > 0){
    //                 // ignTimeMin = INF;
    //                 ignTimeMin = INF;
    //                 // Loop through neighbors
    //                 for(int n = 0; n < 16; n++){
    //                     // find neighbor cell index
    //                     nrow = row + nRow[n];
    //                     ncol = col + nCol[n];
    //                     // std::cout << row << ' ' << col << ' ' << std::endl;
    //                     if ( nrow<0 || nrow>= Rows || ncol<0 || ncol>=  Cols )
    //                         continue;
    //                     ncell = ncol + nrow*Cols;

    //                     ROS = .4 * (1.0 - .6)/(1 - .6 * cos(angle[n]));
    //                     float ignTimeNew = ignTime[ncell] + L_n[n] / ROS;
    //                     ignTimeMin = ignTimeNew*(ignTimeNew < ignTimeMin) + ignTimeMin*(ignTimeNew >= ignTimeMin);
    //                     // if(ignTimeNew < ignTimeMin)
    //                     //     ignTimeNew = ignTimeNew;
    //                     // else if(ignTimeNew >= ignTimeMin)
    //                     //     ignTimeMin = ignTimeMin;
    //                     // std::cout << ignTimeNew << ' ';
    //                 }
    //                 ignTimeNew[cell] = ignTimeMin;
    //             }
    //         }
    //         // std::cout << std::endl;
    //     }

    //     // Swap pointers to loop
    //     float *temp = ignTime;
    //     ignTime = ignTimeNew;
    //     ignTimeNew = temp;
    // }
    //     // std::cout << counter << std::endl;


    // // End Timer
    // gettimeofday(&fin, NULL);
    // double t = fin.tv_usec + fin.tv_sec * 1000000.0;
    // t -= start.tv_usec + start.tv_sec * 1000000.0;
    // t /= 1000000.0;
    // std::cout << "Processing " << T << " cells took " << t << " seconds" << std::endl;

    // for(int i = 0; i < Rows*Cols; i++){
    //     // std::cout << ignTime[i] << " ,";
    //     if(i %Rows == 0 && i !=0){
    //         // std::cout << std::endl;
    //         fout << '\n';
    //     }
    //     fout << (int)ignTime[i] << " ";
    //     // fout << (int)ignTimeNew[i] << " ";
    // }
    // fout.close();

    delete ignTime;
    delete ignTimeNew;
    }   
}