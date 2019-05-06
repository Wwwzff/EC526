#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <chrono>

using namespace std;




int filter(int* pixel, int a, int b, int rgb ,int j){
    int res = 0;
    const int laplace[9] = {
            0,-1, 0,
            -1, 5,-1,
            0,-1, 0
//            0,0,0,
//            0,2,0,
//            0,0,-1
    };
    for(int m = 0; m < 3; m++){
        for(int n = 0; n < 3; n++){
            res += laplace[m+3*n] * pixel[(a+m-1)*j*3+(b+n-1)*3+rgb];
        }
    }
    return res;
}


int* loadPixel(string targetFile, int i, int j){

    int* pixels = (int*) malloc((i+2)*(j+2)*3*sizeof(int));

    ifstream infile(targetFile);
    string curline;
    getline(infile,curline);


    for(int a=1; a<i+1; a++){
        getline(infile,curline);
        istringstream ss{curline};
        using StrIt = istream_iterator<string>;
        vector<string> curRes{StrIt{ss}, StrIt{}};
        for(int b=1;b<j+1;b++){
            for(int rgb=0;rgb<3;rgb++){
                pixels[a*j+b*3+rgb] = stoi(curRes[(b-1)*3+rgb]);
            }
        }
    }
    
    auto borderloop = chrono::steady_clock::now();
#pragma acc parallel loop

    for(int rgb=0;rgb<3;rgb++){
        for(int a=1; a<i+1; a++){
            pixels[a*j+0*3+rgb] = pixels[a*j+1*3+rgb];
            pixels[a*j+(j+1)*3+rgb] = pixels[a*j+j*3+rgb];
        }

        for(int b=1; b<j+1; b++){
            pixels[0*j+b*3+rgb] = pixels[1*j+b*3+rgb];
            pixels[(i+1)*j+b*3+rgb] = pixels[i*j+b*3+rgb];
        }

        pixels[0*j+0*3+rgb] = pixels[1*j+1*3+rgb];
        pixels[(i+1)*j+(j+1)*3+rgb] = pixels[i*j+j*3+rgb];
        pixels[(i+1)*j+0*3+rgb] = pixels[i*j+1*3+rgb];
        pixels[0*j+(j+1)*3+rgb] = pixels[1*j+j*3+rgb];
    }
    auto borderloopend = chrono::steady_clock::now();

    return pixels;

}


int main(int argc, const char * argv[]) {
    auto start = chrono::steady_clock::now();
    const string size = argv[1];
    string targetFile = "./test"+ size +".txt";

    ofstream outFile;
    outFile.open("./result.txt");

    const int i = stoi(argv[1]);
    const int j = stoi(argv[1]);

    int* out = (int*) malloc((i)*(j)*3*sizeof(int));
    const auto pixel = loadPixel(targetFile, i, j);

    auto readinend = chrono::steady_clock::now();
    auto filterloop = chrono::steady_clock::now();

#pragma acc kernels
    {
#pragma acc loop independent
    {
        for (int a = 0; a < i; a++) {
            for (int b = 0; b < j; b++) {
                for (int rgb = 0; rgb < 3; rgb++) {
                    //cout << pixel[a*j*3+b*3+rgb] << endl;
          out[a * j * 3 + b * 3 + rgb] = 5*pixel[(a+1)*j*3 + (b+1)*3 + rgb] - pixel[a*j*3 + (b+1)*3 + rgb] - pixel[(a+1)*j*3 + (b+2)*3 + rgb] - pixel[(a+2)*j*3 + (b+1)*3 + rgb] - pixel[(a+1)*j*3 + b*3 + rgb];
                }
            }
        }
    }
    }
    auto filterloopend = chrono::steady_clock::now();
    auto writefile = chrono::steady_clock::now();
    for(int a=0;a<i;a++){
        for(int b=0;b<j;b++){
            for(int rgb=0;rgb<3;rgb++){
                outFile << out[a*j*3+b*3+rgb] << " ";
            }
        }
        outFile << endl;
    }

    outFile.close();
    auto writeend = chrono::steady_clock::now();


    auto readintime = chrono::duration_cast<chrono::microseconds>(readinend - start);
    auto filtertime = chrono::duration_cast<chrono::microseconds>(filterloopend - filterloop);
    auto writetime = chrono::duration_cast<chrono::microseconds>(writeend - writefile);
    auto total = chrono::duration_cast<chrono::microseconds>(writeend - start);

    cout << "Read in file: " << readintime.count() << endl;
    cout << "filter loop: " << filtertime.count() << endl;
    cout << "writetime: " << writetime.count() << endl;
    cout << "total: " << total.count() << endl;

    return 0;
}
