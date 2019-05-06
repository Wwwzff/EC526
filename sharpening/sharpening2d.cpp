#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int filter(int** matrix, int a, int b){
    int res = 0;
    int laplace[9] = {
            0,-1, 0,
            -1, 5,-1,
            0,-1, 0
//            -1,-1,-1,
//            -1, 9,-1,
//            -1,-1,-1
//            0,0,0,
//            0,2,0,
//            0,0,-1
    };
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            res += laplace[i+3*j] * matrix[a+i-1][b+j-1];
        }
    }
    return res;
}

void storeMatrix(string targetFile, int **R, int **G, int **B, int h, int w){
    ifstream infile(targetFile);
    string curLine;
    getline(infile,curLine);

    //store pixel info to matrix
    for(int i=1; i<h+1; i++){
        getline(infile,curLine);
        istringstream ss{curLine};
        using StrIt = istream_iterator<string>;
        vector<string> curRes{StrIt{ss}, StrIt{}};
//        cout << stoi(curRes[w*3]);
        for(int j=1;j<w-1;j++){
            R[i][j] = stoi(curRes[(j-1)*3]);
            G[i][j] = stoi(curRes[(j-1)*3 + 1]);
            B[i][j] = stoi(curRes[(j-1)*3 + 2]);
        }
    }

    //set border for RGB matrix
    for(int a=1; a<h+1; a++){
        R[a][0] = R[a][1];
        R[a][w+1] = R[a][w];

        G[a][0] = G[a][1];
        G[a][w+1] = G[a][w];

        B[a][0] = B[a][1];
        B[a][w+1] = B[a][w];
    }

    for(int b=1; b<w+1; b++){
        R[0][b] = R[1][b];
        R[h+1][b] = R[h][b];

        G[0][b] = G[1][b];
        G[h+1][b] = G[h][b];

        B[0][b] = B[1][b];
        B[h+1][b] = B[h][b];
    }

    R[0][0] = R[1][1];
    R[h+1][w+1] = R[h][w];
    R[h+1][0] = R[h][1];
    R[0][w+1] = R[1][w];

    G[0][0] = G[1][1];
    G[h+1][w+1] = G[h][w];
    G[h+1][0] = G[h][1];
    G[0][w+1] = G[1][w];

    B[0][0] = B[1][1];
    B[h+1][w+1] = B[h][w];
    B[h+1][0] = B[h][1];
    B[0][w+1] = B[1][w];

}

int main(int argc, const char * argv[]) {

    ofstream outFile;
    outFile.open(R"(C:\Users\zhoub\OneDrive\Desktop\result.txt)");

    //using pixel info in convert txt file to record the height and width
    string targetFile = R"(C:\Users\zhoub\OneDrive\Desktop\test.txt)";
    ifstream infile(targetFile);

    //get the size of image
    string matrixInfo;
    getline(infile, matrixInfo);
    istringstream hw(matrixInfo);
    string height, width;
    hw >> height >> width;
    int h = stoi(height);
    int w = stoi(width);

    cout << h;
    cout << w;

    //initialize rgb matrix
    int** matrixR = new int *[h + 2];
    int** matrixG = new int *[h + 2];
    int** matrixB = new int *[h + 2];
    for(int i=0;i<h + 2;i++){
        matrixR[i] = new int [w + 2];
        matrixG[i] = new int [w + 2];
        matrixB[i] = new int [w + 2];
        for(int j=0;j<w+2;j++){
            matrixR[i][j] = 0;
            matrixG[i][j] = 0;
            matrixB[i][j] = 0;
        }
    }

    //read in pixel info into matrix
    storeMatrix(targetFile, matrixR, matrixG, matrixB, h, w);

//    apply filter output result
    int*** outPixels = new int ** [h + 2];
    for(int a=0; a<h + 2; a++){
        outPixels[a] = new int * [w + 2];
        for(int b=0;b<w + 2;b++){
            outPixels[a][b] = new int [3];
        }
    }

    for(int a=1;a<h + 1;a++){
        for(int b=1;b<w + 1;b++){
            outPixels[a][b][0] = 255-filter(matrixR, a, b);
            outPixels[a][b][1] = 255-filter(matrixG, a, b);
            outPixels[a][b][2] = 255-filter(matrixB, a, b);
            outFile << outPixels[a][b][0] << " ";
            outFile << outPixels[a][b][1] << " ";
            outFile << outPixels[a][b][2] << " ";
        }
        outFile << endl;
    }
    outFile.close();
    return 0;
}