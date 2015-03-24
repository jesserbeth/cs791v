#include <iostream>
#include <fstream>

int main(){
    // vars
    std::ofstream fout;
    fout.open("test.csv");
    int totalTime = 100;
    int timeStep = 3;
    int Rows = 100;
    int Cols = 100;
    int ignMap[Rows*Cols];
    int ignMapStep[Rows*Cols];
    int stepMap[Rows*Cols];
    int burnDist[Rows*Cols];
    for(int i = 0; i < Rows*Cols; i++){
        ignMap[i] = 0;
        ignMapStep[i] = 0;
        stepMap[i] = 0;
        burnDist[i] = 2;
    }
    int row, col, cell, ncell, ncol, nrow;
    /* neighbor's address*/     /* N  NE   E  SE   S  SW   W  NW */
    static int nCol[8] =        {  0,  1,  1,  1,  0, -1, -1, -1};
    static int nRow[8] =        {  1,  1,  0, -1, -1, -1,  0,  1};

    

    //ignite point
    ignMap[5250] = 1;
    bool spreadFire = false;
    // int stepCounter = 0;

    // loop through time:
    for(int t = 0; t < 1000; t++){
        for ( cell=0, row=0; row<Rows; row++ ){
            for ( col=0; col<Cols; col++, cell++ ){
                // check if already "ignited"
                if(ignMap[cell] == 0){
                    continue;
                }
                // std::cout << row << ' ' << ignMap[cell] << std::endl;
                // check neighbors for ignition
                spreadFire = true;
                for(int n = 0; n < 8; n++){
                    nrow = row + nRow[n];
                    ncol = col + nCol[n];
                    if ( nrow<0 || nrow>=Rows || ncol<0 || ncol>=Cols )
                        continue;
                    ncell = ncol + nrow*Cols;

                    // check for already lit
                    if(ignMap[ncell] == 1){
                        continue;
                    }

                    // Burn distance 
                    burnDist[ncell] -= 1;
                    if(burnDist[ncell] == 0){
                        ignMapStep[ncell] = 1;
                        stepMap[ncell] = t;
                        
                    }
                }
            }
            // stepCounter++;
        }
        for(int i = 0; i < Rows*Cols; i++){
            if(ignMapStep[i] == 1)
                ignMap[i] = 1;
        }
    }

    for(int i = 0; i < Rows*Cols; i++){
        // std::cout << stepMap[i] << " ,";
        if(i %Rows == 0 && i !=0){
            // std::cout << std::endl;
            fout << '\n';
        }
        fout << stepMap[i] << " ";
    }
    fout.close();
}